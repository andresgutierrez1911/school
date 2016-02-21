package connectfour;

import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.HBox;
import javafx.scene.layout.HBoxBuilder;
import javafx.scene.layout.VBox;
import javafx.scene.layout.VBoxBuilder;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.stage.Stage;

/**
 * Creates dialog box to display message
 */
public class AlertDialog extends Stage {

    /**
     * Alert box constructor
     * @param String message to display
     */
    public AlertDialog(String message)
    {
        // Create fancy container
        HBox msgContainer = HBoxBuilder.create().spacing(10.0).padding(new Insets(10,10,10,10)).minWidth(300).alignment(Pos.CENTER).build();
        msgContainer.setBackground(new Background(new BackgroundFill(Color.LIGHTSTEELBLUE, null, null)));
        // Create fancy label
        Label msgLabel = new Label(message);
        msgLabel.setFont(Font.font(50));
        msgLabel.setFont(Font.font("",FontWeight.BOLD, 50));
        msgLabel.setAlignment(Pos.CENTER);
        msgContainer.getChildren().addAll(msgLabel);
        // Build button box
        HBox controls = HBoxBuilder.create().spacing(10.0).padding(new Insets(10,10,10,10)).minWidth(300).alignment(Pos.CENTER).build();
        // Create cancel button
        Button btnOk = new Button("OK");
        btnOk.setMinWidth(300);
        btnOk.setMinHeight(25);
        // Create handler for close button... it closes the box
        btnOk.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) {
                // Close the dialog box
                close();
            }
        });

        // Add close button to control box
        controls.getChildren().addAll(btnOk);

        // Assemble the scene
        VBox root = VBoxBuilder.create().spacing(10.0)
                .padding(new Insets(10,10,10,10)).build();
        root.getChildren().addAll(msgContainer, controls);
        Scene s = new Scene(root);
        // Place the scene into this window
        this.setScene(s);
    }
}
