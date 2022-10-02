using lab5.Domain;
using System.Text.Json;
using System.Xml.Linq;
using System.Xml.Serialization;

namespace Serializer
{
    public class Serializer : ISerializer
    {
        public void SerializeByJson(IEnumerable<Company> companies, string fileName)
        {
            var options = new JsonSerializerOptions
            {
                WriteIndented = true
            };
            var json = JsonSerializer.Serialize(companies, options);
            File.WriteAllText(fileName, json);
        }

        public IEnumerable<Company>? DeSerializeByJson(string fileName)
        {
            var json = File.ReadAllText(fileName);
            var companies = JsonSerializer.Deserialize<IEnumerable<Company>>(json);
            return companies;
        }

        public void SerializeByLinq(IEnumerable<Company> companies, string fileName)
        {
            XDocument xdox = new XDocument(new XElement("Companies", companies.Select(c => new XElement("Company",
                new XElement("Name", c.Name),
                new XElement("HrDepartment",
                    new XElement("Vacancies", c.HrDepartment?.Vacancies?.Select(v => new XElement("Vacancy", v))),
                    new XElement("Employees", c.HrDepartment?.Employees?.Select(e => new XElement("Employee",
                        new XElement("Name", e.Name)))))))));
            xdox.Save(fileName);
        }

        public IEnumerable<Company>? DeSerializeByLinq(string fileName)
        {
            var xdoc = XDocument.Load(fileName);
            var companies = xdoc.Element("Companies")?.Elements("Company").Select(x => new Company
            {
                Name = x.Element("Name")?.Value,
                HrDepartment = new HrDepartment
                {
                    Vacancies = x.Element("HrDepartment")?.Element("Vacancies")?.Elements("Vacancy").Select(v => v.Value).ToList(),
                    Employees = x.Element("HrDepartment")?.Element("Employees")?.Elements("Employee").Select(e => new Employee
                    {
                        Name = e.Element("Name")?.Value
                    }).ToList()
                }
            });
            return companies;
        }

        public void SerializeByXml(IEnumerable<Company> companies, string fileName)
        {
            using (Stream stream = File.Open(fileName, FileMode.OpenOrCreate, FileAccess.Write))
            {
                XmlSerializer serializer = new XmlSerializer(companies.GetType());
                serializer.Serialize(stream, companies);
            }
        }

        public IEnumerable<Company>? DeSerializeByXml(string fileName)
        {
            using (Stream stream = File.OpenRead(fileName))
            {
                XmlSerializer serializer = new XmlSerializer(typeof(List<Company>));
                return (IEnumerable<Company>?)serializer.Deserialize(stream);
            }
        }
    }
}
