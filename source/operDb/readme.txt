数据库操作类
	operDbBase为基类，提供统一的接口。接口设置为保护，并添加operDb为友员类。
	operMysql是mysql的操作类，继承自operDbBase。接口设置为保护，并添加operDb为友员类。
	operDb为对外接口类，实际使用时，调用此类操作数据库。
	如果添加其他数据库操作类，继承operDbBase类，接口设置为保护，添加operDb为友员类，并实现相关接口。在operDb类中条添加相应数据库的操作支持
	