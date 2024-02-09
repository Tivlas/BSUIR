CREATE OR REPLACE TRIGGER check_group_id_name_uniqueness
BEFORE INSERT OR UPDATE ON groups
FOR EACH ROW
DECLARE
    duplicate_id_count NUMBER;
    duplicate_name_count NUMBER;
BEGIN
    SELECT COUNT(*) INTO duplicate_id_count
    FROM groups
    WHERE group_id = :new.group_id;

    IF duplicate_id_count > 0 THEN
        RAISE_APPLICATION_ERROR(-20001, 'group_id must be unique');
    END IF;

    SELECT COUNT(*) INTO duplicate_name_count
    FROM groups
    WHERE LOWER(group_name) = LOWER(:new.group_name);

    IF duplicate_name_count > 0 THEN
        RAISE_APPLICATION_ERROR(-20001, 'group_name must be unique');
    END IF;
END;