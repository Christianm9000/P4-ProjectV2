DELIMITER //

CREATE PROCEDURE InsertBitData(IN hexData VARCHAR(255), IN measureId INT)
BEGIN
  DECLARE i, len INT DEFAULT 0;
  DECLARE bitData BINARY(64) DEFAULT '';
  DECLARE hexChar CHAR(1);

  SET len = CHAR_LENGTH(hexData);

  WHILE i < len DO
    SET hexChar = SUBSTRING(hexData, i, 1);
    SET bitData = CONCAT(bitData, LPAD(CONV(hexChar, 16, 2), 4, '0'));
    SET i = i + 1;
  END WHILE;

  INSERT INTO measure_data (id, data) VALUES (measureId, bitData);
END//

DELIMITER ;

DELIMITER //

CREATE TRIGGER AfterInsertIotData
AFTER INSERT ON iot_data
FOR EACH ROW
BEGIN
  CALL InsertBitData(measure_data.data, measure_data.id);
END//

DELIMITER ;