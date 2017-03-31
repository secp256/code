package Animal;

/**
 * Created by zhuliting on 17-3-31.
 */
public class Bird implements Animal {

    private String message;
    private String movess;
    public Bird(String message,String movess){
        this.message=message;
        this.movess=movess;
    }

    @Override
    public void say() {
        //To change body of implemented methods use File | Settings | File Templates.
        System.out.println("bird say: " + message);
    }

    @Override
    public void move() {
        //To change body of implemented methods use File | Settings | File Templates.
        System.out.println("bird move: " + movess);
    }
}