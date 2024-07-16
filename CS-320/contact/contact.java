package contact;

public class contact { //checking if input matches requirements, assigning values and returning contact information

		
		private String contactID;
		private String firstName;
		private String lastName;
		private String phoneNumber;
		private String address;

		
		public contact (String cID, String fName, String lName, String pNumber, String Addrss) {
			
			contactID = cID; //assign contactID to the value of cID
			setfName(fName); //call setfName and pass the vale of fName
			setlName(lName); //call setlName and pass the vale of lName
			setpNumber(pNumber); //call setpNumber and pass the vale of pNumber
			setAddress(Addrss); //call setAddress and pass the vale of Addrss			
			
		}
		
		public void setfName(String fName) { //Method to set the value of firstName and check if the passed value meets requirements
			
			if(fName == null) {
				throw new IllegalArgumentException("Invalid First Name");
			}else if(fName.length() > 10) {
				throw new IllegalArgumentException("Invalid First Name");
			}else {
				firstName = fName;
			}
		}
		
		public void setlName(String lName) { //Method to set the value of lastName and check if the passed value meets requirement
			
			if(lName == null) {
				throw new IllegalArgumentException("Invalid Last Name");
			}else if(lName.length() > 10) {
				throw new IllegalArgumentException("Invalid Last Name");
			}else {
				lastName = lName;
			}
		}
		
		public void setpNumber(String pNumber) { //Method to set the value of phoneNumber and check if the passed value meets requirement
			if(pNumber == null) {
				throw new IllegalArgumentException("Invaled Phone Number");
			}else if (pNumber.length() != 10) {
				throw new IllegalArgumentException("Invaled Phone Number");
			}else {
				phoneNumber = pNumber;
			}
		}
		
		public void setAddress(String Addrss) { //Method to set the value of address and check if the passed value meets requirement
			if(Addrss == null) {
				throw new IllegalArgumentException("Invaled Phone Number");
			}else if (Addrss.length() > 30) {
				throw new IllegalArgumentException("Invaled Phone Number");
			}else {
				address = Addrss;
			}
		}

		public String getfName() { //when called returns the value of firstName
			return firstName;
		}

		public String getlName() { //when called returns the value of lastName
			return lastName;
		}
		
		public String getpNumber() { //when called returns the value of phoneNumber
			return phoneNumber;
		}
		
		public String getAddress() { //when called returns the value of address
			return address;
		}
		
		public String getcontactID() { //when called returns the value of contactID
			return contactID;
		}
}
