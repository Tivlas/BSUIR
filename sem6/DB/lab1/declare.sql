DECLARE
  val NUMBER;
BEGIN
  FOR i IN 1..10000 LOOP
    val := DBMS_RANDOM.VALUE(1, 10000);
 
    INSERT INTO MyTable (val) VALUES (val);
  END LOOP;
END;