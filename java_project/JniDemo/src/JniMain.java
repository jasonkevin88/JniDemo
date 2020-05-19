import java.util.Date;
import java.util.Random;
import java.util.UUID;


public class JniMain {

	//静态方法
	public native static String getStringFromCPP();
	
	//非静态方法
	public native String getStringFromCPP2();
	
	public native void accessField();
	
	public native void accessStaticField();
	
	public native void callInstanceMethod(Animal animal);
	
	//访问Animal的静态方法
	public native String callStaticMethod(Animal animal);
	
	public native String chineseChars(String str);
	
	public native String chineseChars2(String str);
	
	//访问构造方法
	public native Date acceessConstructor();
	
	public native String AcessCacheNewString();
	
	//整型数据在C++中进行排序
	public native void giveArray(int[] inArray);
	
	public native int[][] initInt2DArray(int size);
	
	public native String[] initStringArray(int size);
	
	public native void localRef();
	
	public native void createGlobalRef();
	
	public native String getglobalRef();
	
	public native void delGlobalRef();
	
	public native String createWeakRef();
	
	public native void exception();
	
	public native void cached();
	
	public String key = "key";
	
	public static int count = 9;
	
	static{
		System.loadLibrary("Jni");
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println(getStringFromCPP());
		
		JniMain jniMain = new JniMain();
		System.out.println(jniMain.getStringFromCPP2());
		
		System.out.println("execution C++ method before: key value = "+jniMain.key);
		jniMain.accessField();
		System.out.println("execution C++ method after: key value = "+jniMain.key);
		
		System.out.println("execution C++ method before: count value = "+count);
		jniMain.accessStaticField();
		System.out.println("execution C++ method after: count value = "+count);
		
		Animal animal = new Animal("kevin");
		System.out.println("execution C++ method before animal = "+animal.getName());
		jniMain.callInstanceMethod(animal);
		System.out.println("execution C++ method after animal = "+animal.getName());
		
		
		String uid = jniMain.callStaticMethod(animal);
		System.out.println("execution C++ method after UID = "+uid);
		
		Date date = jniMain.acceessConstructor();
		System.out.println("date : "+date.getTime());
        System.out.println(jniMain.chineseChars("宝宝"));

        System.out.println(jniMain.chineseChars2("宝宝22"));
        
        int[] array = {3,9,2,50,6,13};
        jniMain.giveArray(array);
        for(int i=0; i< array.length; i++) {
        	System.out.println(array[i]);
        }
        
        String[] strArr = jniMain.initStringArray(5);
        for (int i = 0; i < strArr.length; i++) {
			System.out.println("strArr["+i+"] = "+strArr[i]);
		}
        
        int[][] intArr = jniMain.initInt2DArray(4);
        for(int i =0; i < 4; i++) {
        	for(int j = 0; j < 3; j ++) {
        		System.out.println("arr["+i+"]["+j+"] = "+intArr[i][j]);
        	}
        }
        
        jniMain.localRef();
        jniMain.createGlobalRef();
        System.out.println(jniMain.getglobalRef());
        jniMain.delGlobalRef();
        
        try {
        	jniMain.exception();
        	String weakStr = jniMain.createWeakRef();
        	System.out.println("weakStr = "+weakStr);
		} catch (Exception e) {
			// TODO: handle exception
			
			System.out.println(e.toString());
		}
	}

	
	int getRandomInt(int max) {
    	return new Random().nextInt(max);
    }
    
    static String getRandeomUUId() {
    	return UUID.randomUUID().toString();
    }
}
