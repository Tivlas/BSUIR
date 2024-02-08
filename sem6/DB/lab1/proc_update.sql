CREATE OR REPLACE PROCEDURE update_val(p_id IN NUMBER, p_val IN NUMBER) IS
BEGIN
    UPDATE MyTable SET val = p_val WHERE id = p_id;
END;