package server;
import javax.swing.*;
import java.awt.*;
import java.net.*;
import java.io.*;
import java.util.concurrent.*;
import java.util.concurrent.locks.*;
import java.util.*;

 
 public class Server extends JFrame
 {
	 private String[] board = new String[ 9 ]; 
	 private JTextArea outputArea; 
	 private Player[] players; // array of Players
	 private ServerSocket server; // server socket to connect with clients
	 private int currentPlayer; // keeps track of player with current move
	 private final static int PLAYER_X = 0; 
	 private final static int PLAYER_O = 1; 
	 private final static String[] Marks = { "X", "O" }; 
	 private ExecutorService playerThreads; // will run players
	 private Lock mutexLock; // to lock game for synchronization
	 private Condition otherConnected; // to wait for other player
	 private Condition otherTurn; // to wait for other player's turn
 
 
	 public Server()
	 {
		 JFrame newFrame = new JFrame("Tic-Tac-Toe Service");	//set titile of frame
		
		 // create ExecutorService with a thread for each player
		 playerThreads = Executors.newFixedThreadPool( 2 );
		 mutexLock = new ReentrantLock(); //like keyword synchronized
 
		 otherConnected = mutexLock.newCondition();		//like synchronized method .wait()
		 otherTurn = mutexLock.newCondition();		//equal to condition.await();
 
		 for ( int i = 0; i < 9; i++ )
			 board[ i ] = new String( "" ); 
		 players = new Player[ 2 ]; 
		 currentPlayer = PLAYER_X; // set current player to first player
 
		 try
		 {
			 server = new ServerSocket( 1234 ); // set up ServerSocket
		 } 
		 catch ( IOException ioException )
		 {
			 ioException.printStackTrace();
			 System.exit( 1 );
		 } 
 
		 outputArea = new JTextArea(); // create JTextArea for output
		 newFrame.add( outputArea, BorderLayout.CENTER );
		 outputArea.setText( "Server awaiting connections\n" );
		 
		 newFrame.setSize(300 , 300 ); // set size of window
		 newFrame.setVisible( true ); // show window
	 } 
 
	 // wait for two connections so game can be played
	 public void execute()
	 {
	 // wait for each client to connect
		 for ( int i = 0; i < players.length; i++ )
		 {
			 try // wait for connection, create Player, start runnable
			 {
				 players[ i ] = new Player( server.accept(), i ); 
				 playerThreads.execute( players[ i ] ); // execute player runnable
			 } 
			 catch ( IOException ioException )
			 {
				 ioException.printStackTrace();
				 System.exit( 1 );
			 } 
		 } 
		 
		 mutexLock.lock(); // lock game to signal player X's thread
		 
		 try
		 {
			 players[ PLAYER_X ].setSuspended( false ); // resume player X
			 otherConnected.signal(); // wake up player X's thread
		 } 
		 finally
		 {
			 mutexLock.unlock(); // unlock game after signalling player X
		 } 
	 } 
 
 // display message in outputArea
	 private void displayMessage( final String messageToDisplay )
	 {
		// display message from event-dispatch thread of execution
        SwingUtilities.invokeLater(() -> {
            // updates outputArea
            outputArea.append(messageToDisplay); // add message
        });
	 } 
	 
	 // determine if move is valid
	 public boolean validateAndMove( int location, int player )
	 {
		 while ( player != currentPlayer )
		 {
			 mutexLock.lock(); // lock game to wait for other player to go
			 
			 try
			 {
				 otherTurn.await(); // wait for player's turn
			 } 
			 catch ( InterruptedException exception )
			 {
				 exception.printStackTrace();
			 } 
			 finally
			 {
				 mutexLock.unlock(); // unlock game after waiting
			 } 
		 } 
		 
		 // if location not occupied, make move
		 if ( !isMarked( location ) )
		 {
			 board[ location ] = Marks[ currentPlayer ]; // set move on board
			 if(currentPlayer == 0)
				currentPlayer = 1;
			 else if(currentPlayer == 1)
				currentPlayer = 0;
			 
			 // let new current player know that move occurred
			 players[ currentPlayer ].otherPlayerMoved( location );
			 
			 mutexLock.lock(); // lock game to signal other player to go
			 
			 try
			 {
				 otherTurn.signal(); // signal other player to continue
			 }
			 finally
			 {
				 mutexLock.unlock(); // unlock game after signaling
			 } 
			 
			 return true; // notify player that move was valid
		 } 
		 else // move was not valid
			 return false; 
	 } 
 
	 public boolean isMarked( int location )
	 {
	  	 return board[location].equals(Marks[PLAYER_X]) || board[location].equals(Marks[PLAYER_O]);
	 } 
	 
	 // Check if there is 3 of the same marks in a row.
	 public boolean hasWinner() 
	 {
         return (!board[0].isEmpty() && board[0].equals(board[1]) && board[0].equals(board[2]))
                || (!board[3].isEmpty() && board[3].equals(board[4]) && board[3].equals(board[5]))
                || (!board[6].isEmpty() && board[6].equals(board[7]) && board[6].equals(board[8]))
                || (!board[0].isEmpty() && board[0].equals(board[3]) && board[0].equals(board[6]))
                || (!board[1].isEmpty() && board[1].equals(board[4]) && board[1].equals(board[7]))
                || (!board[2].isEmpty() && board[2].equals(board[5]) && board[2].equals(board[8]))
                || (!board[0].isEmpty() && board[0].equals(board[4]) && board[0].equals(board[8]))
                || (!board[2].isEmpty() && board[2].equals(board[4]) && board[2].equals(board[6]));
     }
	
	 public boolean boardFilledUp() 
	 {
         for (int i = 0; i < board.length; ++i) 
         {
             if (board[i].isEmpty()) 
             {
                return false;
             }
         }

         return true;
     }
	 
	 public boolean isGameOver()
	 {
		 return hasWinner() || boardFilledUp();
	 }
 
 	 // private inner class Player manages each Player as a runnable
	 private class Player implements Runnable
	 {
		 private Socket connection; // connection to client
		 private Scanner input; // input from client
		 private Formatter output; // output to client
		 private int playerNumber; // tracks which player this is
		 private String mark; // mark for this player
		 private boolean suspended = true; // whether thread is suspended
		 
		 // set up Player thread
		 public Player( Socket socket, int number )
		 {
			 playerNumber = number; // store this player's number
			 mark = Marks[ playerNumber ]; // specify player's mark
			 connection = socket; // store socket for client
			 
			 try // obtain streams from Socket
			 {
				 input = new Scanner( connection.getInputStream() ); 
				 output = new Formatter( connection.getOutputStream() );
			 } 
			 catch ( IOException ioException )
			 {
				 ioException.printStackTrace();
				 System.exit( 1 );
			 } 
		 } 
	 
		 // send message that other player moved
		 public void otherPlayerMoved( int location )
		 {
			 output.format( "Opponent moved\n" ); 
			 output.format( "%d\n", location ); // send location of move
			 output.flush(); // flush output 
		 } 
		 
		 // control thread's execution
		 public void run()
		 {
		 
			 try
			 {
				 displayMessage( "Player " + mark + " connected\n" );
				 output.format( "%s\n", mark ); // send player's mark
				 output.flush(); // flush output 
				 
				 if ( playerNumber == PLAYER_X )
				 {
				 output.format( "%s\n%s", "Player X connected",
				  "Waiting for another player\n" ); 
				 output.flush(); // flush output 
				 
				 mutexLock.lock(); // lock game to wait for second player
			 
				 try
				 {
					 while( suspended )
					 {
					 otherConnected.await(); // wait for player O
					 } 
				 } 
				 catch ( InterruptedException exception )
				 {
					 exception.printStackTrace();
				 } 
				 finally
				 {
					 mutexLock.unlock(); // unlock game after second player
				 } 
			 
				 // send message that other player connected
				 output.format( "Other player connected. Your move.\n" );
				 output.flush(); // flush output 
			 } 
			 else
			 {
				 output.format( "Player O connected, please wait\n" );
				 output.flush(); // flush output 
			 } 
			 
				 // while game not over
				 while ( !isGameOver() )
				 {
					 int location = 0; // initialize move location
					 
					 if ( input.hasNext() )
						 location = input.nextInt(); // get move location
					 
					 // check for valid move
					 if ( validateAndMove( location, playerNumber ) )
					 {
						 displayMessage( "\nlocation: " + location );
						 output.format( "Valid move.\n" ); // notify client
						 output.flush(); // flush output 
					 } 
					 else 
					 {
						 output.format( "Invalid move, try again\n" );
						 output.flush(); // flush output 
					 } 
				 } 
				 displayMessage("\nPlayer " + Marks[currentPlayer] + " Win!!\n");
				 output.format("Player %s Win!!\n",Marks[currentPlayer]);
				 output.flush();
			 } 
			 finally
			 {
				 try
				 {
					 connection.close(); // close connection to client
				 } 
				 catch ( IOException ioException )
				 {
					 ioException.printStackTrace();
					 System.exit( 1 );
				 } 
			 } 
		 } 
		 
		 // set whether or not thread is suspended
		 public void setSuspended( boolean status )
		 {
			 suspended = status; 
		 } 
	 } 
	 public static void main(String[] args) 
	 {
		Server app = new Server();
		app.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		app.execute();
	 }
 } 
