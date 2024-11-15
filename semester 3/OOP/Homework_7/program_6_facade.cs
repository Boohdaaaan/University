// Task: Provide a unified interface to a set of interfaces in a subsystem, simplifying its usage.
// Pattern: Facade
// HomeTheaterFacade consolidates the operation of multiple components (DVDPlayer, Projector, SoundSystem) into two methods: WatchMovie() and EndMovie().

using System;

namespace FacadeExample
{
    // Subsystem 1
    class DVDPlayer
    {
        public void TurnOn()
        {
            Console.WriteLine("DVD Player is turned ON.");
        }

        public void Play()
        {
            Console.WriteLine("DVD Player is playing the movie.");
        }

        public void TurnOff()
        {
            Console.WriteLine("DVD Player is turned OFF.");
        }
    }

    // Subsystem 2
    class Projector
    {
        public void TurnOn()
        {
            Console.WriteLine("Projector is turned ON.");
        }

        public void TurnOff()
        {
            Console.WriteLine("Projector is turned OFF.");
        }
    }

    // Subsystem 3
    class SoundSystem
    {
        public void TurnOn()
        {
            Console.WriteLine("Sound System is turned ON.");
        }

        public void SetVolume(int level)
        {
            Console.WriteLine($"Sound System volume set to {level}.");
        }

        public void TurnOff()
        {
            Console.WriteLine("Sound System is turned OFF.");
        }
    }

    // Facade
    class HomeTheaterFacade
    {
        private DVDPlayer dvdPlayer;
        private Projector projector;
        private SoundSystem soundSystem;

        public HomeTheaterFacade(DVDPlayer dvdPlayer, Projector projector, SoundSystem soundSystem)
        {
            this.dvdPlayer = dvdPlayer;
            this.projector = projector;
            this.soundSystem = soundSystem;
        }

        public void WatchMovie()
        {
            Console.WriteLine("\nStarting the Home Theater System...");
            dvdPlayer.TurnOn();
            projector.TurnOn();
            soundSystem.TurnOn();
            soundSystem.SetVolume(50);
            dvdPlayer.Play();
        }

        public void EndMovie()
        {
            Console.WriteLine("\nShutting down the Home Theater System...");
            dvdPlayer.TurnOff();
            projector.TurnOff();
            soundSystem.TurnOff();
        }
    }

    // Client Code
    class Program
    {
        static void Main(string[] args)
        {
            // Create subsystems
            DVDPlayer dvdPlayer = new DVDPlayer();
            Projector projector = new Projector();
            SoundSystem soundSystem = new SoundSystem();

            // Create the Facade
            HomeTheaterFacade homeTheater = new HomeTheaterFacade(dvdPlayer, projector, soundSystem);

            // Use the Facade
            homeTheater.WatchMovie();
            homeTheater.EndMovie();

            Console.ReadKey();
        }
    }
}
