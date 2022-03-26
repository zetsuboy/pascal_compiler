namespace HW1
{
    public interface IItem
    {
        int item_count { get; set; }
        int item_id { get; }
        string item_image { get; set; }
        string item_name { get; set; }

        void UpdateCount(int newCount);
    }
}