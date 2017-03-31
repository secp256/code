package cn.itcast.e_anno;

import javax.annotation.Resource;

import org.springframework.stereotype.Component;

@Component("userService")  // userService加入ioc容器
public class UserService {
	
	// 会从IOC容器中找userDao对象，注入到当前字段
	/*
	 * <bean id="" class=""> 
	 *	  <property name="userDao" ref="userDao" />    @Resource相当于这里的配置
	 * </bean>
	 */
	
	@Resource(name = "userDao")
	private UserDao userDao;
	
	public void setUserDao(UserDao userDao) {
		this.userDao = userDao;
	}

	public void save() {
		userDao.save();
	}
}
