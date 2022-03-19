using System;
using System.Collections.ObjectModel;
using System.Linq;
using Xamarin.Forms;

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

        public int item_id
        {
            get;
        }


        public void UpdateCount(int newCount)
        {
            item_count = newCount;
        }
    }

    public partial class MainPage : ContentPage
    {
        public ObservableCollection<Item> items = new ObservableCollection<Item>();

        public MainPage()
        {
            InitializeComponent();
            
            BindableLayout.SetItemsSource(items_container, items);
        }

        private void Button_Clicked(object sender, EventArgs e)
        {
            var itemPage = new AddItemPage();
            itemPage.Disappearing += (_, __) => {
                var selectedItem = itemPage.selectedItem;
                items.Where(x => x.item_name == selectedItem.Name).ToList().ForEach(x =>
                {
                    selectedItem.Count += x.item_count;
                    items.Remove(x);
                });
                items.Add(new Item()
                {
                    item_name = selectedItem.Name,
                    item_count = selectedItem.Count
                });
            };
            Navigation.PushAsync(itemPage);
        }

        private void Button_Clicked_1(object sender, EventArgs e)
        {
            var button = sender as Button;
            foreach(Item i in items)
            {
                if(i.item_id.ToString() == button.ClassId)
                {
                    items.Remove(i);
                    break;
                }
                
            }
        }
    }
}
