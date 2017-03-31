package cn.itcast.e_anno;

import org.springframework.stereotype.Component;

// 把当前对象加入ioc容器
@Component("userDao")   //  相当于bean.xml 【<bean id=userDao class=".." />】
public class UserDao {

	public void save() {
		System.out.println("DB:保存用户!!!");
	}
}
