package com.gamingroom;

import java.util.ArrayList;
import java.util.List;

/**
 * A simple class to hold information about a game
 * 
 * <p>
 * Notice the overloaded constructor that requires
 * an id and name to be passed when creating.
 * Also note that no mutators (setters) defined so
 * these values cannot be changed once a game is
 * created.
 * </p>
 * 
 * @author coce@snhu.edu
 *
 */
public class Game extends Entity{
	long id;
	String name;
	
	private static List<Team> teams = new ArrayList<Team>();
	
	/**
	 * Hide the default constructor to prevent creating empty instances.
	 */
	private Game() {
	}

	/**
	 * Constructor with an identifier and name
	 */
	public Game(long id, String name) {
		this();
		this.id = id;
		this.name = name;
	}
	
	
	for(Team teamIterator : teams){
		if(teamIterator.name == name)
		{
			System.out.println("Team already added");
			return teamIterator;
		}}
	


	@Override
	public String toString() {
		
		return "Game [id=" + id + ", name=" + name + "]";
	}

}
