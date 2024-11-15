// Task: Define an interface for creating objects but let subclasses decide which class to instantiate.
// Pattern: Factory Method
// I defined NotificationFactory to create various types of notifications (email, SMS, push). Each factory decides the type of notification to produce.

using System;

namespace FactoryMethodExample
{
    // Product
    abstract class Notification
    {
        public abstract void NotifyUser();
    }

    // Concrete Products
    class EmailNotification : Notification
    {
        public override void NotifyUser()
        {
            Console.WriteLine("Sending an Email Notification...");
        }
    }

    class SMSNotification : Notification
    {
        public override void NotifyUser()
        {
            Console.WriteLine("Sending an SMS Notification...");
        }
    }

    class PushNotification : Notification
    {
        public override void NotifyUser()
        {
            Console.WriteLine("Sending a Push Notification...");
        }
    }

    // Creator
    abstract class NotificationFactory
    {
        public abstract Notification CreateNotification();
    }

    // Concrete Creators
    class EmailNotificationFactory : NotificationFactory
    {
        public override Notification CreateNotification()
        {
            return new EmailNotification();
        }
    }

    class SMSNotificationFactory : NotificationFactory
    {
        public override Notification CreateNotification()
        {
            return new SMSNotification();
        }
    }

    class PushNotificationFactory : NotificationFactory
    {
        public override Notification CreateNotification()
        {
            return new PushNotification();
        }
    }

    // Client Code
    class Program
    {
        static void Main(string[] args)
        {
            NotificationFactory factory;

            // Create Email Notification
            factory = new EmailNotificationFactory();
            Notification emailNotification = factory.CreateNotification();
            emailNotification.NotifyUser();

            // Create SMS Notification
            factory = new SMSNotificationFactory();
            Notification smsNotification = factory.CreateNotification();
            smsNotification.NotifyUser();

            // Create Push Notification
            factory = new PushNotificationFactory();
            Notification pushNotification = factory.CreateNotification();
            pushNotification.NotifyUser();

            Console.ReadKey();
        }
    }
}
