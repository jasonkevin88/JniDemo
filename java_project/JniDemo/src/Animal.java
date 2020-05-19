
public class Animal {

	protected String name;
    public static int num = 0;
    public Animal(String name) {
        this.name = name;
    }
    public void setName(String name) {
    	this.name = name;
    }
    
    public String getName() {
        return this.name;
    }
    public int getNum() {
        return num;
    }
    
    public static String getUID(String id) {
    	return "10001"+id;
    }
}
