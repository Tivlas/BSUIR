namespace lab5.Domain
{
    public interface ISerializer
    {
        IEnumerable<Company>? DeSerializeByLinq(string fileName);
        IEnumerable<Company>? DeSerializeByXml(string fileName);
        IEnumerable<Company>? DeSerializeByJson(string fileName);
        void SerializeByLinq(IEnumerable<Company> companies, string fileName);
        void SerializeByXml(IEnumerable<Company> companies, string fileName);
        void SerializeByJson(IEnumerable<Company> companies, string fileName);
    }
}
