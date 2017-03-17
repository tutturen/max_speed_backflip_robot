package robot;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

public class RobotController {
	
	BT bluetooth = new BT();
	@FXML private ListView<String> myListView;
	@FXML private Label labelStrategy;
	
	@FXML
	void initialize() {
		listSerialPorts();
		bluetooth.setListener(new BTListener() {
			@Override
			void onCharReceived(char c) {
				System.out.println("Char received: " + c);
				if (c == '1') {
					displayStrategy("MAX SPEED");
				} else if (c == '2') {
					displayStrategy("BACKFLIP");
				}
			}
		});
	}
	
	@FXML
	void listSerialPorts() {
		ObservableList<String> items = FXCollections.observableArrayList(
			bluetooth.getPortNames()
		);
		myListView.setItems(items);	
	}
	
	@FXML
	void openSelectedPort() {
		int index = myListView.getSelectionModel().getSelectedIndex(); 
		bluetooth.openPort(index);
	}
	
	@FXML
	void closePort() {
		bluetooth.closePort();
	}
	
	void displayStrategy(String strategy) {
		labelStrategy.setText(strategy);
	}
	
	@FXML
	void onMaxSpeedClick() {
		System.out.println("max");
		// bluetooth.sendText("MAX");
	}
	
	@FXML
	void onBackflipClick() {
		System.out.println("flip");
		// bluetooth.sendText("FLIP");
	}

}
