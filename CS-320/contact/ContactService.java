package contact;
import java.util.HashMap;
import java.util.Random;
import java.math.BigInteger;

public class ContactService {
	
	private HashMap<String, contact> contactDataBase = new HashMap<String, contact>(); //Create data structure to store unique ID and contact information
	
	public HashMap<String, contact> listContacts(){ //used to expose data structure to public
		return contactDataBase;
	}
	
	public void addContact(String fName, String lName,String pNumber,String Addrss){ //Method to add contacts
		
		String contactID = IDgen(); //assign return from IDgen to a variable
		
		contact Contact = new contact(contactID, fName, lName, pNumber, Addrss); //create new object Contact
		
		if(contactDataBase.containsKey(contactID)){
			throw new IllegalArgumentException("Contact exists"); //Checks to see if ID exists
		}else{contactDataBase.put(contactID, Contact);} //adds contact if ID does not exist
		
	}

	
	public void deleteContact(String contactID) {
		
		if(contactDataBase.isEmpty()){
			throw new IllegalArgumentException("Error, contact list is empty"); //checks if contact list is empty
		}if(contactID == null){
			throw new IllegalArgumentException("Invalid Contact ID"); //checks if input is empty
		}if(!contactDataBase.containsKey(contactID)){
			throw new IllegalArgumentException(contactID + " does not exist"); //checks if contact exists
		}else{contactDataBase.remove(contactID);
			System.out.println(contactID + " removed");} //removes requested contact
		
	}
	
	public void updateContact(String contactID, contact Contact) { //passes contactID and Contact object
		

		if(contactID == null){
			throw new IllegalArgumentException("Invalid Contact ID"); //checks if input is empty
		}if(!contactDataBase.containsKey(contactID)){
			throw new IllegalArgumentException(contactID + " does not exist"); //checks if contact exists
		}if(contactDataBase.isEmpty()){
				throw new IllegalArgumentException("Error, contact list is empty"); //checks if contact list is empty
		}else{contactDataBase.replace(contactID, Contact);
			System.out.println(contactID + " removed");} //updates requested object based on given contactID
	}
	
	private String IDgen() {
	
		String contactID; //create String variable
		
        Random rand = new Random(); //creates a random number
        BigInteger lowerBound = new BigInteger("1000000000"); //sets lowest value
        BigInteger upperBound = new BigInteger("9999999999"); //sets highest value
        int len = upperBound.bitLength(); //sets length of bigint
        BigInteger res = new BigInteger(len,rand); 
        if (res.compareTo(lowerBound) < 0){ //checks if within limit
            res = res.add(lowerBound);
        }
        if (res.compareTo(upperBound) >= 0){ //checks if within limit
            res = res.mod(upperBound).add(lowerBound);
        }
        contactID = res.toString(); //converts to String
        
        return contactID; //returns String contactID
    }

    
}