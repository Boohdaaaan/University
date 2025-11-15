#If DEBUG Then
    Option Explicit On
    Option Strict On
#End If

Module Program
    Sub Main()
        Dim x As Integer = 10
        Dim y As Double = 3.14
        Dim hexValue As Integer = &HFF
        Dim name As String = "Hello World"
        Dim ch As Char = "A"c
        
        ' This is a comment
        REM This is also a comment
        
        If x > 5 Then
            Console.WriteLine(name)
        ElseIf x < 0 Then
            Console.WriteLine("Negative")
        End If
        
        For i As Integer = 1 To 10 Step 2
            Console.WriteLine(i)
        Next
        
        Dim result As Integer = x + y * 2
        Dim comparison As Boolean = (x <> y) And (x >= 5)
        
        ' String concatenation
        Dim fullName As String = "John" & " " & "Doe"
        
        ' Array operations
        Dim numbers() As Integer = {1, 2, 3, 4, 5}
        
        ' Method call
        ProcessData(numbers, result)
    End Sub
    
    Function ProcessData(data() As Integer, multiplier As Integer) As Integer
        Dim sum As Integer = 0
        For Each item In data
            sum += item * multiplier
        Next
        Return sum
    End Function
End Module

