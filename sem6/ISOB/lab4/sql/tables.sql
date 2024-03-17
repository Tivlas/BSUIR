create table roles (
	role_id serial primary key,
	role_name varchar(20) not null, 
	privilege_lvl SMALLINT not null check(privilege_lvl >= 0 and privilege_lvl <= 2)
);

CREATE TABLE users (
  user_id SERIAL PRIMARY KEY,
  username VARCHAR(50) NOT NULL UNIQUE,
  password VARCHAR(50) NOT NULL,
  role_id INTEGER NOT NULL REFERENCES roles(role_id)
);
