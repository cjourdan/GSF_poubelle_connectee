package controller;

import java.util.ArrayList;
import java.util.HashMap;

import model.Sensor;
import model.TrashBin;

public class ConnectionControl {
	private static ConnectionControl instance;
	
	private ArrayList<Sensor> sensors;
	private int nextID;
	
	private ConnectionControl(){
		setSensors(new ArrayList<>());
		nextID = 0;
	}
	
	public static ConnectionControl getInstance(){
		if(instance == null){
			instance = new ConnectionControl();
		}
		return instance;
	}
	
	public int subscribe(){
		sensors.add(null);
		return nextID++;
	}
	
	// Met à jour l'id, la distance actuelle entre le capteur et l'objet et la distance maximum entre le capteur et l'objet
	public void upload(String id, String actualDistance, String maxDistance){
		TrashBin trash = new TrashBin(Integer.parseInt(id), Integer.parseInt(actualDistance), Integer.parseInt(maxDistance));
		getSensors().remove(Integer.parseInt(id));
		getSensors().add(Integer.parseInt(id), trash);
	}

	public ArrayList<Sensor> getSensors() {
		return sensors;
	}

	public void setSensors(ArrayList<Sensor> sensors) {
		this.sensors = sensors;
	}
}
