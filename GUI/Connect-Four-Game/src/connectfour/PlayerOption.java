package connectfour;

import javafx.scene.Node;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;

/**
 * PlayerOption - for extra player data
 * @author Katelyn Schaffer
 */
public class PlayerOption {
    private boolean updated;
    private boolean isDefault;
    private Node defIcon;
    private Image icon;
    
    // PlayerOption constructor
    public PlayerOption(Node initIcon) {
        defIcon = initIcon;
        updated = false;
        isDefault = true;
    }
    
    /* Getters and setters for player data */
    
    public void setUpdateFlag(boolean flag) {
        updated = flag;
    }
    public void setDefaultFlag(boolean flag) {
        isDefault = flag;
    }
    public void setPlayerImage(Image newImg) {
        icon = newImg;
        isDefault = false;
    }
    public boolean needsUpdate() {
        return updated;
    }
    public boolean isDefault() {
        return isDefault;
    }
    public Node getDefaultIcon() {
        // Clone a new icon instance, based on current icon
        Label defIconLbl = (Label) defIcon; // Convert to label
        Label newLabel = new Label(defIconLbl.getText());
        Node newDefIcon = (Node) newLabel;
        return newDefIcon;
    }
    public Node getIcon() {
        // If player is default, return default icon
        if (isDefault) {
            return getDefaultIcon();
        }
        // Otherwise return a new instance of their custom icon
        else {
            // Create a new imageview
            ImageView newImgView = new ImageView();
            newImgView.setImage(icon);
            return newImgView;
        }
    }
}
