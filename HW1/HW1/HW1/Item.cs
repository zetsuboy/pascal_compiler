using System;

namespace HW1
{
    public class Item
    {
        private static int lastId = 0;

        public Item()
        {
            lastId++;
            item_id = lastId;
        }

        public string item_name
        {
            get; set;
        }

        public int item_count
        {
            get; set;
        }
        public string item_image
        {
            get; set;
        }

        public int item_id
        {
            get;
        }


        public void UpdateCount(int newCount)
        {
            item_count = newCount;
        }
    }
}
