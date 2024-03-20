DECLARE
  res        SYS_REFCURSOR;
  v_address  VARCHAR2(100);
  tmp        json_object_t;
  v_name     VARCHAR2(100);
  v_col      VARCHAR2(8000);
  json_data8 CLOB := '{
      "query_type": "DROP TABLE",
      "table": "table2",
      "parameters": "purge"
   }';
  json_data7 CLOB := '{
      "query_type": "CREATE TABLE",
      "table": "table2",
      "columns": [
         {
            "name": "id", "type": "NUMBER"
         },
         {
            "name": "val", "type": "NUMBER"
         }
      ],
      "primary_keys": ["id"],
      "foreign_keys": [{"field": "val", "table": "aaa", "ref_field": "id"}]
   }';
  json_data6 CLOB := '{
      "query_type": "UPDATE",
      "table": "aaa",
      "set": [ {"field": "id", "value": "100"}, {"field": "name", "value": "''Ignat''"}],
      "filter_conditions": ["id = 1"]
    }';
  json_data5 CLOB := '{
  "query_type": "UPDATE",
  "table": "aaa",
  "filter_type": "included",
  "operator": "EXISTS",
  "filter_conditions": {
    "query_type": "SELECT",
    "tables": [
      "bbb"
    ],
    "columns": [
      "*"
    ],
    "filter_conditions": [
      {
        "condition": "bbb.id = aaa.id",
        "operator": "AND"
      }
    ]
  },
  "set": [
    {
      "set_type": "included",
      "field": "id",
      "value": {
        "query_type": "SELECT",
        "columns": [
          "id"
        ],
        "tables": [
          "bbb"
        ],
        "filter_conditions": [
          {
            "condition": "bbb.id = aaa.id",
            "operator": "AND"
          }
        ]
      }
    },
    {
      "field": "name",
      "value": "''Ignat''"
    }
  ]
}';
  json_data3 CLOB := '{
      "query_type": "DELETE",
      "table": "aaa",
      "filter_conditions": ["id = 1"]
    }';
  json_data4 CLOB := '{
      "query_type": "DELETE",
      "table": "aaa",
      "filter_type":"included",
      "operator": "EXISTS",
      "filter_conditions": {
        "query_type": "SELECT",
        "tables": ["bbb"],
        "columns": ["*"],
        "filter_conditions": [
          {
            "condition": "bbb.id = aaa.id",
            "operator": "AND"
          }
        ]
      }
    }';
  json_data2 CLOB := '{
          "query_type": "INSERT",
          "table": "aaa",
          "columns": ["id", "name", "bbb_id"],
          "values_type": "included",
          "values": {
      "query_type": "SELECT",
      "columns": [
        "*"
      ],
      "tables": [
        "bbb"
      ],
      "join_conditions": [],
      "filter_conditions": [
        {
          "condition_type": "included",
          "condition": {
            "query_type": "SELECT",
            "column": "id",
            "tables": [
              "bbb"
            ],
            "filter_conditions": [
              {
                "condition": "name like ''%''",
                "operator": "AND"
              },
              {
                "condition": "id between 2 and 4",
                "operator": "AND"
              }
            ],
            "operator": "IN",
            "search_col": "id"
          },
          "operator": "AND"
        }
      ]
    }
        }';
  json_data1 CLOB := '{
      "query_type": "INSERT",
      "table": "aaa",
      "columns": ["id", "name", "bbb_id"],
      "values": ["4", "''John''", "1"]
    }';
  json_data  CLOB := '{
  "query_type": "SELECT",
  "columns": [
    "*"
  ],
  "tables": [
    "aaa"
  ],
  "join_conditions": [],
  "filter_conditions": [
    {
      "condition_type": "included",
      "condition": {
        "query_type": "SELECT",
        "column": "id",
        "tables": [
          "bbb"
        ],
        "filter_conditions": [
          {
            "condition": "name like ''%''",
            "operator": "AND"
          },
          {
            "condition": "id between 3 and 5",
            "operator": "AND"
          }
        ],
        "operator": "NOT IN",
        "search_col": "id"
      },
      "operator": "AND"
    }
  ]
}';
BEGIN
  res := parse_json(json_data);
 dbms_sql.return_result(res);
  res := parse_json(json_data2);
  res := parse_json(json_data3);
  res := parse_json(json_data4);
  res := parse_json(json_data5);
  res := parse_json(json_data6);
  res := parse_json(json_data7);
  res := parse_json(json_data8);
END;