/**
 * Created by zhuliting on 17-3-31.
 */
public class UserDao4OracleImpl implements UserDao {

    @Override
    public void addUser(User user) {
        System.out.println("UserDao4OracleImpl.addUser() , username="+user.getUsername());
    }
}