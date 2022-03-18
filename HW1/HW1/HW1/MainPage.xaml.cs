using System;
using System.Collections.ObjectModel;
using Xamarin.Forms;

namespace HW1
{
    public class Item
    {

        public Item(string item_name, int item_count, int item_id)
        {
            this.item_name = item_name;
            this.item_count = item_count;
            this.item_id = item_id;
        }

        public string item_name
        {
            get; set;
        }
        
        public int item_count
        {
            get; set;
        }

        public int item_id
        {
            get; set;
        }


        public void UpdateCount(int newCount)
        {
            item_count = newCount;
        }
    }

    public partial class MainPage : ContentPage
    {
        public static ObservableCollection<Item> items = new ObservableCollection<Item>();

        public MainPage()
        {
            InitializeComponent();
            
            BindableLayout.SetItemsSource(items_container, items);
        }

        private void Button_Clicked(object sender, EventArgs e)
        {
            Navigation.PushAsync(new AddItemPage());
        }

        private void Button_Clicked_1(object sender, EventArgs e)
        {
            var button = sender as Button;
            foreach(Item i in items)
            {
                if(i.item_id.ToString() == button.ClassId)
                {
                    MainPage.items.Remove(i);
                    break;
                }
                
            }
        }
    }
}
