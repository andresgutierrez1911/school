package connectfour;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;
import javafx.animation.PathTransition;
import javafx.application.Application;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ContentDisplay;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.HBoxBuilder;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.LineTo;
import javafx.scene.shape.MoveTo;
import javafx.scene.shape.Path;
import javafx.scene.shape.PathElement;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.util.Duration;

/*
 * Connect Four Game
 * Katie Schaffer
 * 9/21/2015
 */
public class ConnectFour extends Application {

    private int minRows, maxRows;       // Min and max number of rows
    private int minCols, maxCols;       // Min and max number of columns
    private int numRows, numCols;       // Current number of rows/columns
    private int[][] board;              // The game board recorder
    private int player;                 // Player indicator
    Node p1Lbl, p2Lbl;                  // Player labels
    Button  btnPlayer1, btnPlayer2;     // Player buttons
    private List<PlayerOption> playerOpts; // List of player data objects
    private PlayerOption pOptions;      // Object containing player data

    private VBox wrapper;               // GUI wrapper
    private HBox gridContainer;         // Grid container
    private GridPane grid;              // The visual grid

    // Some event handlers
    private EventHandler boardBtnClickHandler, playerButtonHandler;


     /**
      * Create Board : Generates the game board array
      * @param numRows : Number of rows
      * @param numCols : Number of columns
      */
    private void CreateBoard(int numRows, int numCols) {
        board = new int[numRows+4][numCols+4];
        for (int row = 0; row < numRows+4; row++) {
            for (int col = 0; col < numCols+4; col++) {
                board[row][col] = 0;
            }
        }
        return;
    }

     /**
      * PopulateGrid : Fills GUI grid with buttons
      * @param rows : Number of rows in grid
      * @param cols : Number of columns in grid
      */
    private void PopulateGrid(int rows, int cols) {
        // Keep it in a grid pane
        grid = new GridPane();
        grid.setGridLinesVisible(false);  // Don't show the grid lines
        for (int buttonNumber = 0; buttonNumber<(rows*cols); buttonNumber++) {
            Button gridBtn = new Button();
            // Button settings
            gridBtn.setMinWidth(30);
            gridBtn.setMinHeight(30);
            gridBtn.setMaxWidth(30);
            gridBtn.setMaxHeight(30);
            gridBtn.setId(Integer.toString(buttonNumber)); // Set button ID
            gridBtn.setOnMouseClicked(boardBtnClickHandler); // Set handler
            // Establish placement of button on the board
            int row = buttonNumber/(cols);
            int col = buttonNumber%(cols);
            GridPane.setConstraints(gridBtn,col,row);
            // Add button to grid
            grid.getChildren().add(gridBtn);
        }
        return;
    }

     /**
      * CreateNewGame : Generates a new game based on
      *    specified rows and columns
      * @param newRows : New number of rows
      * @param newCols : New number of columns
      */
    private void CreateNewGame(int newRows, int newCols) {
        // Set up new board and grid with current row/column options
        CreateBoard(newRows, newCols);
        PopulateGrid(newRows, newCols);
        // Update container
        gridContainer.getChildren().clear();
        gridContainer.getChildren().addAll(grid);
        player = 1;
    }

     /**
     * Method ComputerMove()
     * Handles the generation and animation of computer move
     */
    private void ComputerMove() {
        boolean takingTurn = true;
        int moveIndex;
        Random randGen = new Random();
        // Generate moves until an acceptable turn is made
        while(takingTurn) {
            // Generate a random button number
            moveIndex = randGen.nextInt(numRows*numCols);
            // Calculate coordinates of button (need to be final for handler)
            final int moveRow = moveIndex/(numCols);
            final int moveCol = moveIndex%(numCols);
            // If the selected button is valid, continue with turn
            if (board[moveRow][moveCol] == 0) {
                // Get extra player data (final for use in handler)
                final PlayerOption currPlayer = playerOpts.get(player-1);
                // Get the selected button (final for use in handler)
                final Button currBtn = (Button) grid.getChildren().get(moveIndex);
                Node icon = currPlayer.getIcon();
                grid.getChildren().add(icon);
                // Record the computer's move on the board
                board[moveRow][moveCol] = 2;
                // Place computer's move on the visual grid
                    // All the animation preparation
                Path path = new Path();
                path.getElements().add(new MoveTo(grid.getWidth()/2,grid.getHeight()+5));
                PathElement lineToBtn = new LineTo(currBtn.getLayoutX()+15, currBtn.getLayoutY()+15);
                path.getElements().add(lineToBtn);
                PathTransition animPath = new PathTransition();
                animPath.setPath(path);
                animPath.setDuration(Duration.seconds(1));
                animPath.setNode(icon);
                    // Set events to occur after animation
                animPath.setOnFinished(new EventHandler<ActionEvent>() {
                    @Override
                    public void handle(ActionEvent event) {
                        // Set player icon into button
                        grid.getChildren().remove(icon);
                        currBtn.applyCss();
                        currBtn.setGraphic(currPlayer.getIcon());
                        // Check if there is a win
                        if (CheckForWin(moveRow, moveCol)) {
                            AlertDialog looseMessage =
                                    new AlertDialog("You lost!");
                            looseMessage.initModality(Modality.APPLICATION_MODAL);
                            looseMessage.show();
                        } // If there is no win yet, it is now player 1's turn
                        else {
                            player = 1;
                        }
                     }
                });
                // Finally begin computer animation
                animPath.play();
                // Complete the turn
                takingTurn = false;
                System.out.println("Comp Move: Index["+moveIndex+"] Row["+moveRow+"] Col["+moveCol+"]");
            }
        }
        return;
    }

     /**
     * Method UpdateIcons()
     * Updates changed player buttons
     */
    private void UpdateIcons(int player) {
        int btnIndex;
        Node currNode;
        Button currBtn;
        // Get extra player data
        PlayerOption currPlayer = playerOpts.get(player-1);
        // Update player label
        if (player == 1)    {
            p1Lbl = currPlayer.getIcon();
            btnPlayer1.setGraphic(p1Lbl); } // if Player 1
        else {
            p2Lbl = currPlayer.getIcon();
            btnPlayer2.setGraphic(p2Lbl); } // if Player 2
        // Loop through every space on the board
        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {
                // If a space belongs to the player, modify it
                if (board[row][col] == player) {
                    // Calculate button index in visual grid
                    btnIndex = (row*numCols)+col;
                    // Get the button and update its image
                    currNode = grid.getChildren().get(btnIndex);
                    currBtn = (Button) currNode;
                    // Add player's icon to button
                    currBtn.setGraphic(currPlayer.getIcon());

                } // Finished updating a button
            }
        } // End loop through board
        // The player has been updated; unflag it
        currPlayer.setUpdateFlag(false);
    }

    // ** Start Method ** //
    @Override
    public void start(Stage primaryStage) {
        /* Initialize max and min values */
        minRows = 4;
        maxRows = 20;
        minCols = 4;
        maxCols = 40;
        numRows = 10;
        numCols = 10;

        /* Define event handlers */
        // Handler for board buttons
        boardBtnClickHandler = new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent event) {
                // Only handle if it is player 1's turn
                if (player == 1) {
                    // Put all the grid elements into a list
                    ObservableList<Node> btnList = grid.getChildren();
                    // Create an iterator to look through that list
                    Iterator iter = btnList.iterator();
                    Node currNode; // Current item in list
                    Button currBtn;// Current button
                    // Initialize button index tracker
                    int btnIndex = 0; // Accommodate for extra item in list
                    // Search for the button that was clicked
                    while (iter.hasNext()) {
                        currNode = (Node) iter.next(); // Get current button
                        // If the clicked button is found, proceed to handle
                        if (currNode == event.getSource()) {
                            // Based on its index, find coordinates on board
                            int btnRow = btnIndex/(numCols);
                            int btnCol = btnIndex%(numCols);

                            System.out.println("Your Move: Index["+btnIndex+"] Row["+btnRow+"] Col["+btnCol+"]");

                            // If the selected coordinates are valid, proceed
                            if (board[btnRow][btnCol] == 0) {
                                // Mark board area as "clicked" by player 1
                                board[btnRow][btnCol] = 1;
                                // Get the button itself
                                currBtn = (Button) currNode;
                                // Access to extra player data from player list
                                PlayerOption currPlayer = playerOpts.get(player-1);
                                // Add icon to button
                                currBtn.setGraphic(currPlayer.getIcon());
                                CheckForWin(btnRow, btnCol);
                                // Now check for win
                                if (CheckForWin(btnRow, btnCol)) {
                                    // If play 1 won, display message
                                        // (and lock out the user)
                                    player = 2;
                                    AlertDialog winMessage =
                                            new AlertDialog("You win!");
                                    winMessage.initModality(Modality.APPLICATION_MODAL);
                                    winMessage.showAndWait();
                                } // If there is no win yet,
                                        //it's the computer's turn
                                else {
                                    player = 2;
                                    // Queue the computer's move
                                    ComputerMove();
                                }
                                return; // Escape button search loop
                            } // End of adding move
                            // Invalid move
                            System.out.println("Invalid move!");
                        } // End of handling button
                        // Increment button index tracker
                        btnIndex++;
                    } // End of loop through grid elements
                } // End of player 1's turn
            } // End of event handler
        }; // End end

        // Handler for player buttons
        playerButtonHandler = new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent event) {
                // Get player indicator from source button user data
                Button btn = (Button) event.getSource();
                int player = (int) btn.getUserData();
                // Access player data from list
                PlayerOption selectedPlayer = playerOpts.get(player-1);
                // Open player dialog box (locking out the other window)
                PlayerDialog diagPlayerOpts = new PlayerDialog(selectedPlayer);
                diagPlayerOpts.initModality(Modality.APPLICATION_MODAL);
                diagPlayerOpts.showAndWait();
                // If an update was made, make update to whole player
                if (selectedPlayer.needsUpdate()) {
                    UpdateIcons(player);
                }
            }
        };


        /* GUI wrapper/root */
        wrapper = new VBox();
        wrapper.setPadding(new Insets(10,10,10,10));
        wrapper.setBackground(new Background(new BackgroundFill(Color.LIGHTSTEELBLUE, null, null)));

        /* Controls */
        HBox controls = HBoxBuilder.create().spacing(10.0)
                .padding(Insets.EMPTY).build();

        // Create labels and siders
        Label lblRows = new Label("Rows:");
        Label lblCols = new Label("Columns:");
        Slider sliderRows = new Slider();
        Slider sliderCols = new Slider();
        // Set slider settings
        sliderRows.setValue(numRows);
        sliderCols.setValue(numCols);
        sliderRows.setMin(minRows);
        sliderRows.setMax(maxRows);
        sliderCols.setMin(minCols);
        sliderCols.setMax(maxCols);
        // Create "new game" button and handler
        Button btnNewGame = new Button("New Game");
        btnNewGame.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                // Update row and column numbers with slider values
                numRows = (int) sliderRows.getValue();
                numCols = (int) sliderCols.getValue();
                // Set up new game
                CreateNewGame(numRows, numCols);
                primaryStage.sizeToScene();
            }
        });
        // Add control elements to controls
        controls.getChildren().addAll(btnNewGame, lblRows, sliderRows,
                lblCols, sliderCols);

        /* Player Options*/
        // Build player option container
        HBox playerOptions = HBoxBuilder.create()
                .spacing(10.0).padding(Insets.EMPTY).build();
        // Create player option buttons
        p1Lbl = new Label("X");
        p2Lbl = new Label("Y");
        btnPlayer1 = new Button("Player 1: ");
        btnPlayer1.setMinHeight(40);
        btnPlayer1.setMaxHeight(40);
        btnPlayer1.setGraphic(p1Lbl);
        btnPlayer1.setContentDisplay(ContentDisplay.RIGHT);
        btnPlayer2 = new Button("Player 2: ");
        btnPlayer2.setMinHeight(40);
        btnPlayer2.setMaxHeight(40);
        btnPlayer2.setGraphic(p2Lbl);
        btnPlayer2.setContentDisplay(ContentDisplay.RIGHT);
        // Attach player indicators to buttons as user data
        int player1Indicator = 1;
        int player2Indicator = 2;
        btnPlayer1.setUserData(player1Indicator);
        btnPlayer2.setUserData(player2Indicator);
        // Attach appropriate handler to buttons
        btnPlayer1.setOnMouseClicked(playerButtonHandler);
        btnPlayer2.setOnMouseClicked(playerButtonHandler);
        // Add player option elements to player options box
        playerOptions.getChildren().addAll(btnPlayer1, btnPlayer2);

        /* Initial Game Setup */
        // Set up the board and visual grid with default rows/columns
        CreateBoard(numRows, numCols);
        PopulateGrid(numRows, numCols);
        // Put the visual grid into the container
        gridContainer = new HBox();
        gridContainer.setPadding(new Insets(10,0,10,0));
        gridContainer.getChildren().addAll(grid);
        // Initialize player list and add players
        playerOpts = new LinkedList<PlayerOption>();
        pOptions = new PlayerOption(p1Lbl);
        playerOpts.add(pOptions);
        pOptions = new PlayerOption(p2Lbl);
        playerOpts.add(pOptions);
        //
        // Start with player 1
        player = 1;

        /* Assemble and display GUI */
        wrapper.getChildren().addAll(controls, gridContainer, playerOptions);
        Scene scene = new Scene(wrapper);
        // Prepare and display primary stage
        primaryStage.setTitle("Four In A Row");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }

    /**
     * Method CheckForWin
     * @param moveRow , the row number of a move
     * @param moveCol , the column number of a move
     * @return whether or not the move is a winning move
     */
    private boolean CheckForWin(int moveRow, int moveCol) {
     boolean isWin = false;                 // Whether or not it's a win
     int inARow = 1;                        // In-a-row counter
     // Initialize scanners to specified
     //     move location
     int scanRow = moveRow;                 // Row scanner
     int scanCol = moveCol;                 // Column scaner
     // Get the player's marker
     int pTile = board[moveRow][moveCol];   // The player's marker
     int checkRadius = 3;                   // Searchable radius around button
     int check = 0;                         // Number of checks in a direction
                                            //  that have been made so far

     // Check right
     while ( (scanCol+1<numCols)
             && (board[moveRow][scanCol+1]==pTile)
             && (check<checkRadius) ) {
         inARow++;
         scanCol++;
         check++;
     }
     // Check left, if 4 aren't found yet
     if (inARow<4) {
        scanRow = moveRow;
        scanCol = moveCol;
        check = 0;
        while ( (scanCol-1>=0)
                && (board[moveRow][scanCol-1]==pTile)
                && (check<checkRadius) ) {
            inARow++;
            scanCol--;
            check++;
        }
     }

     // Check down, if 4 aren't found yet
     if (inARow<4) {
        // Reset in-a-row count because this is a new axis
        inARow = 1;
        scanRow = moveRow;
        scanCol = moveCol;
        check = 0;
        while ( (scanRow+1<numRows)
                && (board[scanRow+1][moveCol]==pTile)
                && (check<checkRadius) ) {
            inARow++;
            scanRow++;
            check++;
        }
     }
     // Check up, if 4 aren't found yet
     if (inARow<4) {
        scanRow = moveRow;
        scanCol = moveCol;
        check = 0;
        while ( (scanRow-1>=0)
                && (board[scanRow-1][moveCol]==pTile)
                && (check<checkRadius) ) {
            inARow++;
            scanRow--;
            check++;
        }
     }

     // Check up-left, if 4 aren't found yet
     if (inARow<4) {
        // Reset in-a-row count because this is a new axis
        inARow = 1;
        scanRow = moveRow;
        scanCol = moveCol;
        check = 0;
        while ( (scanRow-1>=0) && (scanCol-1>=0)
                && (board[scanRow-1][scanCol-1]==pTile) && (check<checkRadius) ) {
            inARow++;
            scanRow--;
            scanCol--;
            check++;
        }
     }

     // Check down-right, if 4 aren't found yet
     if (inARow<4) {
        scanRow = moveRow;
        scanCol = moveCol;
        check = 0;
        while ( (scanRow+1<numRows) && (scanCol+1<numCols)
                && (board[scanRow+1][scanCol+1]==pTile) && (check<checkRadius) ) {
            inARow++;
            scanRow++;
            scanCol++;
            check++;
        }
     }

     // Check up-right, if 4 aren't found yet
     if (inARow<4) {
        // Reset in-a-row count because this is a new axis
        inARow = 1;
        scanRow = moveRow;
        scanCol = moveCol;
        check = 0;
        while ( (scanRow-1>=0) && (scanCol+1<numCols)
                && (board[scanRow-1][scanCol+1]==pTile) && (check<checkRadius) ) {
            inARow++;
            scanRow--;
            scanCol++;
            check++;
        }
     }

     // Check down-left, if 4 aren't found yet
     if (inARow<4) {
        scanRow = moveRow;
        scanCol = moveCol;
        check = 0;
        while ( (scanRow+1<numRows) && (scanCol-1>=0)
                && (board[scanRow+1][scanCol-1]==pTile) && (check<checkRadius) ) {
            inARow++;
            scanRow++;
            scanCol--;
            check++;
        }
     }
     // If 4-in-a-row is found, we have a winnder
     if (inARow>=4) {
         isWin = true;
     }
     return isWin; // return the result
    }

}
