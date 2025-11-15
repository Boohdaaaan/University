Module TestErrors
    Sub Test()
        Dim x As Integer = 123
        Dim y As Double = 3.14159
        Dim hex As Integer = &HABC
        Dim str As String = "Hello"
        Dim ch As Char = "X"c
        
        ' Valid comment
        REM Another comment
        
        #If DEBUG Then
            Console.WriteLine("Debug mode")
        #End If
        
        ' Test operators
        If x <> y And x >= 10 Then
            Console.WriteLine("OK")
        End If
        
        ' Unrecognized characters: @ $ %
        Dim test@ As Integer = 5
        Dim price$ As String = "100"
        Dim percent% As Integer = 50
    End Sub
End Module

