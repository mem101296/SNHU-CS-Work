package test;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import contact.ContactService;
import contact.contact;

class contactServiceTest {

	@Test
	void testaddContact() {
		ContactService contactServ = new ContactService(); //creates new object
		contactServ.addContact("Mike", "Martin", "4446761212", "123 address"); //add contact
		Assertions.assertTrue(!contactServ.listContacts().isEmpty()); //checks if contact was added	
	}
	
	@Test
	void testdeletContact() {
		String testdelID = "";
		ContactService contactServ = new ContactService(); //creates new object
		
		Assertions.assertThrows(IllegalArgumentException.class, ()->{ 
			contactServ.deleteContact("");}); //checks if empty
		
		contactServ.addContact("Nick", "Desk", "2224540101", "456 address"); //add contact
	
		for (contact cant : contactServ.listContacts().values()) {
			testdelID = cant.getcontactID();
		}
		
		Assertions.assertThrows(IllegalArgumentException.class, ()->{ 
			contactServ.deleteContact(null);}); //checks if null
		
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			contactServ.deleteContact("09999");}); //checks if wrong ID throws error
		
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			contactServ.deleteContact("0123456789");}); //deletes created contact
		
		contactServ.deleteContact(testdelID); //deletes created task
		Assertions.assertThrows(IllegalArgumentException.class, ()->{ 
			contactServ.deleteContact("");}); //tries to remove using an empty String
	
		}
	
	@Test
	void testupdateContact() {
		String testupdateID = "";
		ContactService contactServ = new ContactService();
		
		
		Assertions.assertThrows(IllegalArgumentException.class, ()->{ 
			contactServ.updateContact(null, null);}); //checks if null
		
		Assertions.assertThrows(IllegalArgumentException.class, ()->{ 
			contactServ.updateContact("0112229", null);}); //checks if null
		
		Assertions.assertThrows(IllegalArgumentException.class, ()->{ 
			contactServ.updateContact("0123456789", new contact("9876543210", "Joe", "End", "0123456789", "abcdfecdtg"));}); //tests contact exist check
		
		new contact("0123456789", "Nick", "Desk", "2224540101", "456 address"); //add contact
	
		contactServ.addContact("Nick", "Desk", "2224540101", "456 address"); //add contact
		
		for (contact cant : contactServ.listContacts().values()) {
			testupdateID = cant.getcontactID();
		}
		 
		contactServ.updateContact(testupdateID, new contact(testupdateID, "Mike", "Martin", "9876543210", "town mcTown")); //checks contact update
	

	}
}


