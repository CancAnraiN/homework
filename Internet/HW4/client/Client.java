package client;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.net.Socket;
import java.net.InetAddress;
import java.io.IOException;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import java.util.Formatter;
import java.util.Scanner;
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;

public class Client extends JFrame implements Runnable{
	private JTextField idField;		//display player's ,ark
	private JTextArea showArea;
	private JPanel panelboard;	// panel for tic-tac-toe board
	private JPanel panel2;	// panel to hold board
	private Square board[][];// tic-tac-toe board
	private Square currentSquare;
	private Socket connection; //connect to server
	private Scanner input;		//get input from server
	private Formatter output;	//output to server
	private String Host;	//get the host name(IP address)
	private String myMark;	//self own marks
	private boolean myTurn;
	private final String X_mark = "X";	//mark for first player
	private final String O_mark = "O";	//mark for sencond player
	
	public Client(String host) 
	{
		JFrame newFrame = new JFrame("Tic-Tac-Toe Client");
		
		Host = host;
		//setting the text frame which show the message to client
		showArea = new JTextArea(4 , 30);
		showArea.setEditable(false);
		newFrame.add(new JScrollPane(showArea) , BorderLayout.SOUTH);
		
		panelboard = new JPanel();
		panelboard.setLayout(new GridLayout(3,3,0,0));
		
		board = new Square[3][3]; // create board
		for(int row = 0; row < board.length; row++)
		{
			for(int column = 0;column < board[row].length; column++)
			{
				//create square
				board[row][column] = new Square(" ",row * 3 + column);
				panelboard.add(board[row][column]); // add square
			}
		}
		
		//set up textfield
		idField = new JTextField(); 
		idField.setEditable(false);
		newFrame.add(idField , BorderLayout.NORTH);
		
		panel2 = new JPanel();	//contain panelboard
		panel2.add(panelboard,BorderLayout.CENTER);
		newFrame.add(panel2,BorderLayout.CENTER);
		
		newFrame.setSize(300, 225);
		newFrame.setVisible(true);
		
		startClient();
	}
	
	//start client's thread
	public void startClient() {
		// connect server and get streams
		try {
			connection = new Socket(InetAddress.getByName(Host),1234); //make connection
			//get input and output stream
			input = new Scanner(connection.getInputStream());
			output = new Formatter(connection.getOutputStream());
		}
		catch(IOException ioException) {
			System.out.println(ioException.toString());
		}
		ExecutorService worker = Executors.newFixedThreadPool(1);	//create client thread
		worker.execute(this);		//execute client
	}
	
	 /**
     * The Player’s run method controls the information that is sent to and
     * received from the client.
     */
	public void run() {
		myMark = input.nextLine();	//get X or O 
		
		SwingUtilities.invokeLater(() -> {
            // display player's mark
            idField.setText("You are player \"" + myMark + "\"");
        });
		
		myTurn = (myMark.equals(X_mark));
		while(true)
		{
			if(input.hasNextLine())
				process(input.nextLine());
		}
	}
	
	private void process(String message) {
		if(message.equals("Valid move."))
		{
			display("Valid move, please wait.\n");
			setMark(currentSquare , myMark);
		}
		else if(message.equals("Invalid moved, try again."))
		{
			display(message + "\n");
			myTurn = true;
		}
		else if(message.equals("Opponent moved"))
		{
			int location = input.nextInt();
			input.nextLine();	//ignore newline
			int row = location / 3;
			int col = location % 3;
			
			setMark(board[row][col],(myMark.equals(O_mark) ? X_mark : O_mark));
			display("Opponent moved. Your turn.\n");
			myTurn = true;
		}
		else
		{
			display(message + "\n");
		}
	}
	private void display(final String message) {
		SwingUtilities.invokeLater(
				new Runnable() {
					public void run(){
						showArea.append(message);
				}
			}
		);
	}
	
	private void setMark(final Square marked,final String theMark) {
		SwingUtilities.invokeLater(
				new Runnable() {
					public void run(){
						marked.setMark(theMark);
				}
			}
		);
	}
	
	public void sendClickedSquare(int location) {
		if(myTurn)
		{
			output.format("%d\n",location);
			output.flush();
			myTurn = false;
		}
	}
	public void setCurrentSquare(Square square) 
	{
		currentSquare = square;
	}
	
	private class Square extends JPanel
	{
		private String mark; // mark to be drawn in this square
		private int location; // location of square
	
		public Square( String squareMark, int squareLocation )
		{
			mark = squareMark; // set mark for this square
			location = squareLocation; // set location of this square
	
			addMouseListener(new MouseAdapter()
			{
				public void mouseReleased( MouseEvent e )
				{
					setCurrentSquare( Square.this ); // set current square
					// send location of this square
					sendClickedSquare( getSquareLocation() );
				} 
			});  
		} 
 
	 // return preferred size of Square
		public Dimension getPreferredSize() 
		{
			return new Dimension( 30, 30 ); // return preferred size
		} 
 
		public Dimension getMinimumSize()
		{
			return getPreferredSize(); // return preferred size
		} 
 
		public void setMark( String newMark )
		{
			mark = newMark; // set mark of square
			repaint(); // repaint square
		} 
	
		public int getSquareLocation()
		{
			return location; // return location of square
		} 
		
		// draw Square
		public void paintComponent( Graphics g )
		{
			super .paintComponent( g );
	
			g.drawRect( 0, 0, 29, 29 ); // draw square
			g.drawString( mark, 11, 20 ); // draw mark
		} 
	} 
	public static void main(String[] args) 
	{
		Client app;
		app = new Client("localhost");
		app.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
}
