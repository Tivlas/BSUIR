CREATE OR REPLACE TRIGGER update_c_val_trigger
AFTER INSERT OR UPDATE OR DELETE ON students
FOR EACH ROW
DECLARE
  new_group_id NUMBER;
  old_group_id NUMBER;
BEGIN
  IF INSERTING THEN
    UPDATE groups
    SET c_val = c_val + 1
    WHERE id = :NEW.gr_id;
    
  ELSIF UPDATING THEN
    new_group_id := :NEW.gr_id;
    old_group_id := :OLD.gr_id;
    
    IF new_group_id <> old_group_id THEN
      UPDATE groups
      SET c_val = c_val + 1
      WHERE id = new_group_id;
      
      UPDATE groups
      SET c_val = c_val - 1
      WHERE id = old_group_id;
    END IF;
    
  ELSIF DELETING THEN
    UPDATE groups
    SET c_val = c_val - 1
    WHERE id = :OLD.gr_id;
  END IF;
END;
