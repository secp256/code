package Animal;

import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

/**
 * Created by zhuliting on 17-3-31.
 */
public class ATest {
    public static void main(String[] args) {
        ApplicationContext applicationContext = null;

        try {
            applicationContext = new ClassPathXmlApplicationContext("spring-config.xml");

        } catch (BeansException e) {
            e.printStackTrace();
        }

        Animal animal = applicationContext.getBean("cat", Cat.class);
        animal.say();
        animal.move();

        Animal animal1 = applicationContext.getBean("bird", Bird.class);
        animal1.say();
        animal1.move();

    }
}
