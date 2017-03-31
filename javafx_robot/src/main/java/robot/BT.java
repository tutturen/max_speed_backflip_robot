package robot;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.List;
import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;

public class BT {
	
	private SerialPort btPort;
	private SerialPort[] serialPorts;
	private BTListener listener;
	
	public BT() {
		this.refresh();
	}
	
	List<String> getPortNames() {
		refresh();
    	String[] portNames = new String[this.serialPorts.length];
		for(int i=0; i < this.serialPorts.length; i++) {
			portNames[i] = this.serialPorts[i].getDescriptivePortName();
		};
		return Arrays.asList(portNames);
	}
	
	void refresh() {
		this.serialPorts = SerialPort.getCommPorts();
	}
	
	void openPort(int index) {
		if (index <= serialPorts.length) {
			btPort = serialPorts[index];
			btPort.openPort();
			btPort.addDataListener(new SerialPortDataListener() {
			   public int getListeningEvents() { 
				   return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
			   }
			   public void serialEvent(SerialPortEvent event)
			   {
				  if (listener == null
				   || event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE) {
					  return;
				  }
			      byte[] newData = new byte[btPort.bytesAvailable()];
			      btPort.readBytes(newData, newData.length);
			      for (char c : (new String(newData)).toCharArray()) {
			    	  listener.onCharReceived(c);
			      }
			   }
			});
		}
	}
	
	void closePort() {
		if (btPort == null || !btPort.isOpen()) {
			return;
		}
		btPort.closePort();
	}
	
	void sendText(String text) {
		if (btPort == null || !btPort.isOpen()) {
			System.out.println("Can not send text, port is not open");
			return;
		}
		String s = text + "\r\n";
		byte[] buffer = new byte[s.length()];
		for(int i=0;i<s.length();i++) {
			buffer[i] = (byte) s.charAt(i);
		};
		OutputStream out = btPort.getOutputStream();
		try {
			out.write(buffer, 0, s.length());
		} catch (IOException e) {
			e.printStackTrace();
		} 
	}
	
	void setListener(BTListener listener) {
		this.listener = listener;
	}
	
	

}
