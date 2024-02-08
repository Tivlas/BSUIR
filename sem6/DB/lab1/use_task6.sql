DECLARE
    v_salary NUMBER := 1000;
    v_percent NUMBER := 10;
    v_remuneration NUMBER;
BEGIN
    v_remuneration := calculate_total_compensation(v_salary, v_percent);
    IF v_remuneration IS NOT NULL THEN
        DBMS_OUTPUT.PUT_LINE('Общее вознаграждение за год: ' || v_remuneration);
    END IF;
END;
