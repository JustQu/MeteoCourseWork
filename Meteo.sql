CREATE TABLE `users` (
  `id` INT UNIQUE PRIMARY KEY NOT NULL AUTO_INCREMENT,
  `location_id` INT,
  `login` VARCHAR(32) UNIQUE NOT NULL,
  `password` VARCHAR(128) NOT NULL,
  `group_id` INT
);

CREATE TABLE `groups` (
  `group_id` INT UNIQUE PRIMARY KEY NOT NULL AUTO_INCREMENT,
  `group_name` VARCHAR(20) UNIQUE NOT NULL,
  `group_level` INT NOT NULL
);

CREATE TABLE `locations` (
  `location_id` INT UNIQUE PRIMARY KEY NOT NULL AUTO_INCREMENT,
  `location_name` VARCHAR(128)
);

CREATE TABLE `stations` (
  `station_id` INT UNIQUE PRIMARY KEY NOT NULL AUTO_INCREMENT,
  `location_id` INT NOT NULL,
  `temperature` INT,
  `humidity` INT,
  `pressure` INT,
  `wind_speed` INT,
  `wind_direction_id` INT,
  `precipitation_id` INT
);

CREATE TABLE `indications_history` (
  `indication_id` INT UNIQUE PRIMARY KEY NOT NULL AUTO_INCREMENT,
  `station_id` INT NOT NULL,
  `time` TIMESTAMP NOT NULL,
  `temperature` INT,
  `humidity` INT,
  `pressure` INT,
  `wind speed` INT,
  `wind_direction_id` INT,
  `precipitation_id` INT
);

CREATE TABLE `precipitations` (
  `precipitation_id` INT UNIQUE PRIMARY KEY NOT NULL AUTO_INCREMENT,
  `precipitation` VARHCAR(64)
);

CREATE TABLE `wind_directions` (
  `wind_direction_id` INT UNIQUE PRIMARY KEY NOT NULL AUTO_INCREMENT,
  `wind_direction` VARCHAR(32)
);

ALTER TABLE `users` ADD FOREIGN KEY (`group_id`) REFERENCES `groups` (`group_id`);

ALTER TABLE `users` ADD FOREIGN KEY (`location_id`) REFERENCES `locations` (`location_id`);

ALTER TABLE `stations` ADD FOREIGN KEY (`location_id`) REFERENCES `locations` (`location_id`);

ALTER TABLE `indications_history` ADD FOREIGN KEY (`wind_direction_id`) REFERENCES `wind_directions` (`wind_direction_id`);

ALTER TABLE `stations` ADD FOREIGN KEY (`wind_direction_id`) REFERENCES `wind_directions` (`wind_direction_id`);

ALTER TABLE `indications_history` ADD FOREIGN KEY (`precipitation_id`) REFERENCES `precipitations` (`precipitation_id`);

ALTER TABLE `stations` ADD FOREIGN KEY (`precipitation_id`) REFERENCES `precipitations` (`precipitation_id`);

ALTER TABLE `indications_history` ADD FOREIGN KEY (`station_id`) REFERENCES `stations` (`station_id`);
