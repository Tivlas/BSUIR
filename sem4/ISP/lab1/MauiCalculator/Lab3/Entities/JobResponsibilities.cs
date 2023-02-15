using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SQLite;

namespace MauiCalculator.Lab3.Entities
{
    [Table("JobResponsibilities")]
    public class JobResponsibilities
    {
        [PrimaryKey, AutoIncrement]
        public int ID { get; set; }

        [MaxLength(100)]
        public string Description { get; set; }

        [Indexed]
        public int EmployeePositionID { get; set; }
    }
}
