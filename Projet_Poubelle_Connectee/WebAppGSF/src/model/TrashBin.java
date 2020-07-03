package model;

public class TrashBin extends Sensor{
	private int actualDistance;
	private int maxDistance;
	
	// Initialisation de l'id, de la distance actuelle et de la distance maximale
	public TrashBin(int id, int act, int max){
		this.setID(id);
		this.setActualDistance(act);
		this.setMaxDistance(max);
	}

	public int getActualDistance() {
		return actualDistance;
	}

	public void setActualDistance(int actualDistance) {
		this.actualDistance = actualDistance;
	}

	public int getMaxDistance() {
		return maxDistance;
	}

	public void setMaxDistance(int maxDistance) {
		this.maxDistance = maxDistance;
	}
	public String toString(){
		return actualDistance + " " + maxDistance;
	}
}
