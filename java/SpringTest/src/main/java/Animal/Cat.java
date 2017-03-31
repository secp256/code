package Animal;

import Animal.Animal;

/**
 * Created by zhuliting on 17-3-31.
 */
public class Cat implements Animal {
    @Override
    public void move() {
        System.out.println("cat move: 4 legs");
    }

    @Override
    public void say() {
        System.out.println("cat say: miao miao");
    }
}
