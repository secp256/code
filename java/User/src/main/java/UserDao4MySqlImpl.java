/**
 * Created by zhuliting on 17-3-31.
 */
public class UserDao4MySqlImpl implements UserDao {

    public UserDao4MySqlImpl () {
    }

    @Override
    public void addUser(User user) {
        System.out.println("UserDao4MySqlImpl.addUser()  , username="+user.getUsername());
    }
}
