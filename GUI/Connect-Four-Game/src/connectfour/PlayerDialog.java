package connectfour;

import java.io.File;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.image.Image;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.HBox;
import javafx.scene.layout.HBoxBuilder;
import javafx.scene.layout.VBox;
import javafx.scene.layout.VBoxBuilder;
import javafx.scene.paint.Color;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

/*
 * Player dialog box with player options
 */
public class PlayerDialog extends Stage {

    /**
     * constructor to build the dialog box
     * @param PlayerOption object full of player data
     */
    public PlayerDialog(PlayerOption playerData)
    {
        // Initialize default player icon based on player
        Node defPlayerIcon = playerData.getDefaultIcon();
        Label playerIconLbl = (Label) defPlayerIcon; // Convert to label

        // Create group of icon options
        ToggleGroup iconOptions = new ToggleGroup();
        // Build radio buttons for default and image
        RadioButton radioDefaultIcon = new RadioButton(playerIconLbl.getText());
        RadioButton radioCustom = new RadioButton("Choose Image");
        // Add radio buttons to group
        radioDefaultIcon.setToggleGroup(iconOptions);
        radioCustom.setToggleGroup(iconOptions);
        // Initialize radio group to default
        iconOptions.selectToggle(radioDefaultIcon);

        // Build button box
        HBox controls = HBoxBuilder.create().spacing(10.0).padding(new Insets(10,10,10,10)).build();
        controls.setBackground(new Background(new BackgroundFill(Color.LIGHTSTEELBLUE, null, null)));
        // Create cancel button
        Button cancel = new Button("Cancel");
        cancel.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                // Close the dialog box
                close();
            }
        });
        // Create OK button
        Button btnAccept = new Button("Accept");
        btnAccept.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                // If user selected "Choose Image", make it so
                if (iconOptions.getSelectedToggle()==radioCustom) {
                    // Open file chooser to select image
                    FileChooser myChooser = new FileChooser();
                    File myFile = myChooser.showOpenDialog(null);
                    Image myImg = new Image("file:"+myFile.getPath(),
                            30, 30, false, false);
                    // Set the player's new icon
                    playerData.setPlayerImage(myImg);
                    playerData.setUpdateFlag(true);
                } // Otherwise, use the default icon.
                  // Make sure it actually needs changing
                else if (iconOptions.getSelectedToggle()==radioDefaultIcon
                        && !playerData.isDefault()) {
                    playerData.setDefaultFlag(true);
                    playerData.setUpdateFlag(true);
                }
                // Close the dialog box
                close();
            }
        });
        // Put buttons into control box
        controls.getChildren().addAll(cancel, btnAccept);

        // Assemble the scene
        VBox root = VBoxBuilder.create().spacing(10.0)
                .padding(new Insets(10,10,10,10)).build();
        root.getChildren().addAll(radioDefaultIcon, radioCustom, controls);
        Scene s = new Scene(root);
        // Place the scene into this window
        this.setScene(s);
    }
}
