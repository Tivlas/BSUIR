using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SQLite;

namespace MauiCalculator.Lab3.Entities
{
    [Table("EmployeePosition")]
    public class EmployeePosition
    {
        [PrimaryKey, AutoIncrement]
        public int ID { get; set; }

        [MaxLength(50)]
        public string Name { get; set; }

        public int Salary { get; set; }
        
    }
}
