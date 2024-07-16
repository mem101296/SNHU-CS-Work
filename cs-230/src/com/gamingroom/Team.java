package com.gamingroom;

import java.util.ArrayList;
import java.util.List;

/**
 * A simple class to hold information about a team
 * <p>
 * Notice the overloaded constructor that requires
 * an id and name to be passed when creating.
 * Also note that no mutators (setters) defined so
 * these values cannot be changed once a team is
 * created.
 * </p>
 * @author coce@snhu.edu
 *
 */
public class Team {
	long id;
	String name;
	
private static List<Game> games = new ArrayList<Game>();
	
	/*
	 * Constructor with an identifier and name
	 */
	public Team(long id, String name) {
		this.id = id;
		this.name = name;
	}

	{
		
		for(Game gameIterator : games){
			if(gameIterator.name == name)
			{
				System.out.println("Team already added");
				return gameIterator;
			}
		}
	}



	@Override
	public String toString() {
		return "Team [id=" + id + ", name=" + name + "]";
	}
}
