// The `EmailSender` class violates the Single Responsibility Principle.
// The `EmailSender` class is responsible for:
// 1. Sending an email.
// 2. Logging messages to the console.


using System;

// Class representing an email
class Email
{
    public string Theme { get; set; }
    public string From { get; set; }
    public string To { get; set; }
}

// Interface for logging
interface ILogger
{
    void Log(string message);
}

// Implementation of logging to the console
class ConsoleLogger : ILogger
{
    public void Log(string message)
    {
        Console.WriteLine(message);
    }
}

// Class responsible for sending emails
class EmailSender
{
    private readonly ILogger _logger;

    // Constructor to inject the logger dependency
    public EmailSender(ILogger logger)
    {
        _logger = logger;
    }

    public void Send(Email email)
    {
        // ... sending logic ...
        _logger.Log($"Email from '{email.From}' to '{email.To}' was sent");
    }
}

// Main program
class Program
{
    static void Main(string[] args)
    {
        Email e1 = new Email() { From = "Me", To = "Vasya", Theme = "Who are you?" };
        Email e2 = new Email() { From = "Vasya", To = "Me", Theme = "vacuum cleaners!" };
        Email e3 = new Email() { From = "Kolya", To = "Vasya", Theme = "No! Thanks!" };
        Email e4 = new Email() { From = "Vasya", To = "Me", Theme = "washing machines!" };
        Email e5 = new Email() { From = "Me", To = "Vasya", Theme = "Yes" };
        Email e6 = new Email() { From = "Vasya", To = "Petya", Theme = "+2" };

        // Inject the console logger into the EmailSender
        ILogger logger = new ConsoleLogger();
        EmailSender emailSender = new EmailSender(logger);

        emailSender.Send(e1);
        emailSender.Send(e2);
        emailSender.Send(e3);
        emailSender.Send(e4);
        emailSender.Send(e5);
        emailSender.Send(e6);

        Console.ReadKey();
    }
}
