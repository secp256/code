/**
 * Created by zhuliting on 17-3-31.
 */
public class UserManagerImpl implements UserManager {
    private UserDao userDao;

    public UserManagerImpl(UserDao userDao) {
        this.userDao = userDao;
    }

    @Override
    public void addUser(User user) {
        this.userDao.addUser(user);
    }
}