package test;

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import contact.contact;

class contactTest {
	

	@Test
	void testContactGetters() {
		contact Contact = new contact ("0123456789", "Name", "End", "0123456789", "abcdfecdtg"); //creates object
		assertTrue(Contact.getfName().equals("Name")); //gets fname, returns true if matches
		assertTrue(Contact.getlName().equals("End")); //gets lname, returns true if matches
		assertTrue(Contact.getpNumber().equals("0123456789")); //gets pnumber, returns true if matches
		assertTrue(Contact.getAddress().equals("abcdfecdtg")); //gets address, returns true if matches
		assertTrue(Contact.getcontactID().equals("0123456789")); //gets contactID, returns true if matches
	}
	
	@Test
	void testNull() { //checks all types of null entries
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			new contact(null, null, null, null, null);
		});
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			new contact("0123456789", null, null, null, null);
		});
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			new contact("0123456789", "Name", null, null, null);
		});
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			new contact("0123456789", "Name", "End", null, null);
		});
		Assertions.assertThrows(IllegalArgumentException.class, () ->{
			new contact("0123456789", "Name", "End", "0123456789", null);
		});
	}

	@Test
	void testContactSetters() {
		contact Contact = new contact ("0123456789", "Name", "End", "0123456789", "abcdfecdtg"); //creates an object
		Contact.setfName("Mike"); //changes fName
		Contact.setlName("Martin"); //changes lName
		Contact.setAddress("169 Worcet St"); //changes Address
		Contact.setpNumber("9876543210"); //changes pNumber
		assertTrue(Contact.getfName().equals("Mike"));  //gets fname, returns true if matches
		assertTrue(Contact.getlName().equals("Martin")); //gets lname, returns true if matches
		assertTrue(Contact.getAddress().equals("169 Worcet St")); //gets address, returns true if matches
		assertTrue(Contact.getpNumber().equals("9876543210")); //gets pnumber, returns true if matches
	}

}
