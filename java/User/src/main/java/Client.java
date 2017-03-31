import org.springframework.beans.factory.BeanFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;

/**
 * Created by zhuliting on 17-3-31.
 */

public class Client {
    public static void main(String[] args) {

        User user = new User();
        user.setUsername("张三");
        user.setPassword("123456");

/*        UserDao userDao = new UserDao4MySqlImpl();

        UserManager userManager = new UserManagerImpl(userDao);
        userManager.addUser(user);*/

        BeanFactory factory = new ClassPathXmlApplicationContext("spring-config.xml");
        UserManager userManager = (UserManager)factory.getBean("userManager");
        userManager.addUser(user);

    }
}