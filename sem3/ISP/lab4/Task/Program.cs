using System;
using Task;

namespace Task
{
    public class Program
    {
        public static void Main(string[] args)
        {
            List<StoreGood> storeGoods = new List<StoreGood>
            {

                new StoreGood("5Milk", 20, true),
                new StoreGood("6Bread", 10, false),
                new StoreGood("2Meat", 50, true),
                new StoreGood("4Fish", 30, false),
                new StoreGood("3Potato", 5, true),
                new StoreGood("1Tomato", 7, false)
            };
            var fstream = File.Create("savedCollectionInfo.bin");
            fstream.Close();
            FileService fileService = new FileService();

            try
            {
                fileService.SaveData(storeGoods, fstream.Name);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.GetType());
                Console.WriteLine(e.Message);
                Console.WriteLine(e.StackTrace);
            }

            string newFileName = "savedCollectionInfo2.bin";
            try
            {
                File.Move(fstream.Name, newFileName);
            }
            catch (IOException)
            {
                File.Delete(newFileName);
                File.Move(fstream.Name, newFileName);
            }

            List<StoreGood>? storeGoods2 = null;
            try
            {
                storeGoods2 = new List<StoreGood>(fileService.ReadFile("savedCollectionInfo2.bin"));
            }
            catch (Exception e)
            {
                Console.WriteLine(e.GetType());
                Console.WriteLine(e.Message);
                Console.WriteLine(e.StackTrace);
            }

            var sortedByName = storeGoods2?.OrderBy(x => x, new MyCustomComparer());
            var sortedByCost = storeGoods2?.OrderBy(x => x.Cost);

            Console.WriteLine("\nNot sorted:");
            foreach (var item in storeGoods)
            {
                Console.WriteLine(item);
            }

            Console.WriteLine("\nSorted by name:");
            foreach (var item in sortedByName!)
            {
                Console.WriteLine(item);
            }


            Console.WriteLine("\nSorted by cost:");
            foreach (var item in sortedByCost!)
            {
                Console.WriteLine(item);
            }
        }
    }
}
