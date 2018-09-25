Public Class DealFileDB
    '''' 所有接口说明
    '
    ' 用于初始化可以初始化的DealFileDB对象的变量
    '' Public Function DealFileDBInit() As Boolean
    '
    ' 用于初始化关于路径的变量
    '' Public Sub PathYMDInit()
    '
    ' 用于测试保存文件的路径是否已经存在，不存在返回False，存在返回True
    '' Public Function TestSaveFilePath() As Boolean
    '
    ' 用于创建指定的路径的文件夹
    '' Public Function CreateSaveFilePath() As Boolean
    '
    ' 用于删除指定的文件
    '' Public Function DeleteSaveFile(ByVal filePathName As String) As Boolean
    '
    ' 用于删除指定的文件夹
    '' Public Function DeleteSaveDirectory(ByVal directoryPathName As String) As Boolean
    '
    ' 用于按行读取出指定的txt文件的内容并保存在变量TxtDataLineStr
    '' Public Function ReadTxtAllLine(Optional ByVal fileName As String = "") As Boolean
    '
    ' 用于将TxtDataLineStr写入指定的txt文件中
    '' Public Function WriteTxtAllLine(Optional ByVal fileName As String = "") As Boolean
    '
    ' 用于提取出某一行的第i个键值对应的字符串,字符串形如:"1 S001 NG 0 1 0 1",i从0开始
    '' Public Function ExtractKeyStr(ByVal singleRow As String, ByVal i As Integer) As String
    '
    ' 用于寻找TxtDataLineStr中某键值的一行，并返回此行数，失败返回0
    '' Public Function SelectDB(ByVal i As Integer, ByVal keyStr As String) As Integer
    '
    ' 用于删除TxtDataLineStr中指定的某一行数据，成功返回True，失败返回False
    '' Public Function DeleteDB(ByVal index As Integer) As Boolean
    '
    ' 用于更新TxtDataLineStr中的ID
    '' Public Function UpdateIDDB() As Boolean
    '
    ' 用于增加TxtDataLineStr中的一行记录
    '' Public Function AddDB(ByVal record As String) As Boolean
    '
    ' 用于安全地增加TxtDataLineStr中的一行记录, flag为1表示即使一样需要替换，为0表示一样就不要替换
    '' Public Function SafeAddDB(ByVal record As String, Optional ByVal flag As Integer = 1) As Boolean
    '
    ' 用于将TxtDataLineStr变量中的内容存储在TxtDataTable变量
    '' Public Function SaveDataTable(Optional ByVal columnStr As String = "") As Boolean
    '
    ' 用于将TxtDataTable变量中的内容存储在TxtDataLineStr变量
    '' Public Function SaveDataLine() As Boolean
    '
    ' 用于将TxtDataLineStr变量中的内容存储在TxtDataTable变量,再与DataGridView窗口变量绑定
    '' Public Function BindTDLS2DGV(ByRef DGV As System.Windows.Forms.DataGridView) As Boolean
    '
    ' 用于将DataGridView窗口变量中的内容存储在TxtDataTable变量,再与TxtDataLineStr变量绑定
    '' Public Function BindDGV2TDLS(ByRef DGV As System.Windows.Forms.DataGridView) As Boolean
    '
    ' 用于创建或者更新保存文件，如果表头发生变化，需要及时更新
    '' Public Function UpdateSaveFile(Optional ByVal tableFirst As String = "") As Boolean
    '
    ' 用于删除TxtDataLineStr变量中某一列的所有数据，列数从0开始计数
    '' Public Function DeleteDBCol(ByVal col As Integer) As Boolean
    '
    ' 用于判断某一列是否存在于TxtDataLineStr变量的列名中，列为字符串
    '' Public Function IsColumnIn(ByVal colNameStrSet As String, ByVal colNameStr As String) As Integer
    '
    ' 用于按照需要的新的列名更新TxtDataLineStr变量
    '' Public Function UpdataDBWithNewColName(ByVal colNameStr As String) As Boolean
    '
    '
    '''' 定义变量
    '
    '' 用于创建保存路径
    '' 保存的文件为txt文件，保存的文件路径为: Year/Month/*.txt
    Public PathStr As String = ""                             '用于保存文件的总文件夹名字，例如："D:/DataBase/"
    Public SavePathStr As String = ""                         '用于保存文件的文件夹名字，例如："D:/DataBase/YYYY/MM/"
    Public SaveFilePathStr As String = ""                     '用于保存文件的文件的全名字，例如："D:/DataBase/YYYY/MM/YYYYMMDD"
    Public SaveFileNameStr As String = ""                     '用于保存文件的文件的名字，例如:YYYYMMDD
    Public YearStr As String = ""                             '用于保存当前年的字符串
    Public MonthStr As String = ""                            '用于保存当前月的字符串
    Public YMDStr As String = ""                              '用于保存当前年月日的字符串
    '
    '' 用于保存txt内容
    Public TxtDataLineStr() As String                         '用于保存从txt中读取出内容的字符串变量数组
    Public TxtDataLineNum As Integer                          '用于保存从txt中读取出内容的字符串的行数
    Public TxtDataFirstLine As String                         '用于保存从txt中读取出内容的第一行字符串
    Public TxtDataTable As DataTable                          '用于保存从txt中读取出内容的DataTable的变量
    '
    '' 用于初始化可以初始化的DealFileDB对象的变量
    Public Function DealFileDBInit() As Boolean
        Try
            PathYMDInit()
            TxtDataTable = New DataTable()
            TxtDataLineNum = 0
            TxtDataFirstLine = ""
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于初始化关于路径的变量
    Public Sub PathYMDInit()
        YMDStr = Format(System.DateTime.Now, "yyyyMMdd")
        YearStr = YMDStr.Substring(0, 4)
        MonthStr = YMDStr.Substring(4, 2)
        PathStr = "D:/DataBase/"
        SavePathStr = PathStr & YearStr & "/" & MonthStr & "/"
        SaveFileNameStr = YMDStr
        SaveFilePathStr = SavePathStr & SaveFileNameStr
    End Sub
    '
    '' 用于测试保存文件的路径是否已经存在，不存在返回False，存在返回True
    Public Function TestSaveFilePath() As Boolean
        If Not System.IO.Directory.Exists(SavePathStr) Then
            Return False
        End If
        Return True
    End Function
    '
    '' 用于创建指定的路径的文件夹
    Public Function CreateSaveFilePath() As Boolean
        Try
            System.IO.Directory.CreateDirectory(SavePathStr)
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于删除指定的文件
    Public Function DeleteSaveFile(ByVal filePathName As String) As Boolean
        Try
            If Not System.IO.File.Exists(filePathName) Then
                Return False
            End If
            System.IO.File.Delete(filePathName)
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于删除指定的文件夹
    Public Function DeleteSaveDirectory(ByVal directoryPathName As String) As Boolean
        Try
            Dim dirDirectory() As String = System.IO.Directory.GetDirectories(directoryPathName)
            Dim fileDirectory() As String = System.IO.Directory.GetFiles(directoryPathName)
            '
            For Each tempFile As String In fileDirectory
                If Not DeleteSaveFile(tempFile) Then
                    Return False
                End If
            Next
            '
            For Each tempDir As String In dirDirectory
                If Not DeleteSaveDirectory(tempDir) Then
                    Return False
                End If
            Next
            '
            System.IO.Directory.Delete(directoryPathName)
            '
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于按行读取出指定的txt文件的内容并保存在变量TxtDataLineStr
    Public Function ReadTxtAllLine(Optional ByVal fileName As String = "") As Boolean
        Try
            If fileName = "" Then
                fileName = SaveFilePathStr
                If fileName = "" Then
                    MsgBox("欲打开的文件名为空，请确认!")
                    Return False
                End If
            End If
            TxtDataLineStr = System.IO.File.ReadAllLines(fileName)
            Dim i As Integer = 0
            While i <= TxtDataLineStr.GetUpperBound(0)
                TxtDataLineStr(i) = Trim(TxtDataLineStr(i))
                i = i + 1
            End While
            TxtDataLineNum = TxtDataLineStr.GetUpperBound(0)
            TxtDataFirstLine = TxtDataLineStr(0)
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于将TxtDataLineStr写入指定的txt文件中
    Public Function WriteTxtAllLine(Optional ByVal fileName As String = "") As Boolean
        Try
            If fileName = "" Then
                fileName = SaveFilePathStr
                If fileName = "" Then
                    MsgBox("欲保存的文件名为空，请确认!")
                    Return False
                End If
            End If
            System.IO.File.WriteAllLines(fileName, TxtDataLineStr)
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于提取出某一行的第i个键值对应的字符串,字符串形如:"1 S001 NG 0 1 0 1",i从0开始
    Public Function ExtractKeyStr(ByVal singleRow As String, ByVal i As Integer) As String
        Try
            Dim tempStr() As String = singleRow.Split(" ")
            Return tempStr(i)
        Catch ex As Exception
            Return ""
        End Try
    End Function
    '
    '' 用于寻找TxtDataLineStr中某键值的一行，并返回此行数，失败返回0
    Public Function SelectDB(ByVal i As Integer, ByVal keyStr As String) As Integer
        Try
            Dim iIter As Integer = 1
            While (iIter <= TxtDataLineStr.GetUpperBound(0))
                If ExtractKeyStr(TxtDataLineStr(iIter), i) = keyStr Then
                    Exit While
                End If
                iIter = iIter + 1
            End While
            If iIter > TxtDataLineStr.GetUpperBound(0) Then
                iIter = 0
            End If
            Return iIter
        Catch ex As Exception
            Return 0
        End Try
    End Function
    '
    '' 用于删除TxtDataLineStr中指定的某一行数据，成功返回True，失败返回False
    Public Function DeleteDB(ByVal index As Integer) As Boolean
        Try
            Dim TxtDataLineStrCopy(TxtDataLineStr.GetUpperBound(0) - 1) As String
            'ReDim Preserve TxtDataLineStrCopy(TxtDataLineStr.Length - 1)
            Dim iCopy As Integer = 0
            Dim i As Integer = 0
            While i <= TxtDataLineStr.GetUpperBound(0)
                If i <> index Then
                    TxtDataLineStrCopy(iCopy) = TxtDataLineStr(i)
                    iCopy = iCopy + 1
                End If
                i = i + 1
            End While
            TxtDataLineStr = TxtDataLineStrCopy
            UpdateIDDB()
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于更新TxtDataLineStr中的ID
    Public Function UpdateIDDB() As Boolean
        Try
            Dim i As Integer = 1
            While i <= TxtDataLineStr.GetUpperBound(0)
                Dim tempStr() As String = TxtDataLineStr(i).Split(" ")
                tempStr(0) = i.ToString
                TxtDataLineStr(i) = ""
                For Each item As String In tempStr
                    TxtDataLineStr(i) = TxtDataLineStr(i) & item & " "
                Next
                TxtDataLineStr(i) = TxtDataLineStr(i).Substring(0, TxtDataLineStr(i).Length - 1)
                i = i + 1
            End While
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于增加TxtDataLineStr中的一行记录
    Public Function AddDB(ByVal record As String) As Boolean
        Try
            Dim index As Integer = TxtDataLineStr.GetUpperBound(0) + 1
            Dim TxtDataLineStrCopy(index) As String
            'ReDim Preserve TxtDataLineStrCopy(TxtDataLineStr.Length - 1)
            Dim i As Integer = 0
            While i <= TxtDataLineStr.GetUpperBound(0)
                TxtDataLineStrCopy(i) = TxtDataLineStr(i)
                i = i + 1
            End While
            TxtDataLineStrCopy(i) = index.ToString & " " & record
            TxtDataLineStr = TxtDataLineStrCopy
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于安全地增加TxtDataLineStr中的一行记录, flag为1表示即使一样需要替换，为0表示一样就不要替换
    Public Function SafeAddDB(ByVal record As String, Optional ByVal flag As Integer = 1) As Boolean
        Try
            Dim existFlag As Integer = 0
            Dim recordStr() As String = record.Split(" ")
            Dim i As Integer = 1
            While i <= TxtDataLineStr.GetUpperBound(0)
                If ExtractKeyStr(TxtDataLineStr(i), 1) = recordStr(0) Then
                    existFlag = 1
                    Exit While
                End If
                i = i + 1
            End While

            If existFlag = 0 Then
                AddDB(record)
            Else
                If (flag = 1) And (Not (TxtDataLineStr(i) = record)) Then
                    ' 1 s001 ng 1 0 0 1
                    ' 0   1   2 3 4 5 6
                    '   s001 ng 1 1 0 0
                    '     0   1 2 3 4 5
                    Dim tempStr As String = ""
                    Dim tempRecord() As String = TxtDataLineStr(i).Split(" ")
                    Dim maxI As Integer = tempRecord.GetUpperBound(0)
                    Dim iTemp As Integer = 3
                    While iTemp <= maxI
                        tempRecord(iTemp) = recordStr(iTemp - 1)
                        iTemp = iTemp + 1
                    End While
                    iTemp = 0
                    While iTemp <= maxI
                        tempStr = tempStr & tempRecord(iTemp) & " "
                        iTemp = iTemp + 1
                    End While
                    tempStr = tempStr.Substring(0, tempStr.Length - 1)
                    TxtDataLineStr(i) = tempStr
                End If
            End If
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于将TxtDataLineStr变量中的内容存储在TxtDataTable变量
    Public Function SaveDataTable(Optional ByVal columnStr As String = "") As Boolean
        Try
            ' 添加列名
            Dim columnStrArr() As String
            If columnStr = "" Then
                columnStrArr = TxtDataLineStr(0).Split(" ")
            Else
                columnStrArr = columnStr.Split(" ")
            End If

            TxtDataTable = New DataTable("TxtDataLineStr")

            For Each item As String In columnStrArr
                TxtDataTable.Columns.Add(item, System.Type.GetType("System.String"))
            Next
            ' 添加每一行
            Dim i As Integer = 1
            While i <= TxtDataLineStr.GetUpperBound(0)
                Dim tempStr() As String = TxtDataLineStr(i).Split(" ")
                Dim tempRow As DataRow = TxtDataTable.NewRow()
                Dim j As Integer = 0
                While j <= tempStr.GetUpperBound(0)
                    tempRow(j) = tempStr(j)
                    j = j + 1
                End While
                TxtDataTable.Rows.Add(tempRow)
                i = i + 1
            End While
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于将TxtDataTable变量中的内容存储在TxtDataLineStr变量
    Public Function SaveDataLine() As Boolean
        Try
            Dim TxtDataLineStrCopy(TxtDataTable.Rows.Count) As String
            Dim iCopy As Integer = 0
            Dim tempStr As String = ""
            ' 存储所有列信息
            tempStr = ""
            Dim ColNameStrArr(TxtDataTable.Columns.Count - 1) As String
            Dim colIndex As Integer = 0
            For Each colItem As DataColumn In TxtDataTable.Columns
                tempStr = tempStr & colItem.ColumnName & " "
                '
                ColNameStrArr(colIndex) = colItem.ColumnName
                colIndex = colIndex + 1
            Next
            tempStr = tempStr.Substring(0, tempStr.Length - 1)
            TxtDataLineStrCopy(iCopy) = tempStr
            iCopy = iCopy + 1
            ' 存储所有行信息
            Dim rowIndex As Integer = 0
            For Each rowItem As DataRow In TxtDataTable.Rows
                tempStr = ""
                rowIndex = 0
                While rowIndex <= ColNameStrArr.GetUpperBound(0)
                    tempStr = tempStr & rowItem.Item(rowIndex).ToString & " "
                    rowIndex = rowIndex + 1
                End While
                tempStr = tempStr.Substring(0, tempStr.Length - 1)
                TxtDataLineStrCopy(iCopy) = tempStr
                iCopy = iCopy + 1
            Next
            TxtDataLineStr = TxtDataLineStrCopy
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于将TxtDataLineStr变量中的内容存储在TxtDataTable变量,再与DataGridView窗口变量绑定
    Public Function BindTDLS2DGV(ByRef DGV As System.Windows.Forms.DataGridView) As Boolean
        Try
            If Not SaveDataTable() Then
                Return False
            End If
            DGV.DataSource = TxtDataTable
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于将DataGridView窗口变量中的内容存储在TxtDataTable变量,再与TxtDataLineStr变量绑定
    Public Function BindDGV2TDLS(ByRef DGV As System.Windows.Forms.DataGridView) As Boolean
        Try
            Dim TxtDataTableCopy As DataTable = New DataTable("TxtDataTable")
            ' 存储所有列的信息
            Dim colIndex As Integer = 0
            Dim colMaxIndex As Integer = DGV.Columns.Count - 1
            While colIndex <= colMaxIndex
                '对照模式:TxtDataTable.Columns.Add(item, System.Type.GetType("System.String"))
                TxtDataTableCopy.Columns.Add(DGV.Columns(colIndex).Name.ToString(), System.Type.GetType("System.String"))
                colIndex = colIndex + 1
            End While
            ' 存储所有行的信息
            Dim rowIndex As Integer = 0
            Dim rowMaxIndex As Integer = DGV.Rows.Count - 2
            While rowIndex <= rowMaxIndex
                Dim tempRow As DataRow = TxtDataTableCopy.NewRow()
                colIndex = 0
                While colIndex <= colMaxIndex
                    tempRow(colIndex) = DGV.Rows(rowIndex).Cells(colIndex).Value.ToString
                    '
                    colIndex = colIndex + 1
                End While
                TxtDataTableCopy.Rows.Add(tempRow)
                '
                rowIndex = rowIndex + 1
            End While
            TxtDataTable = TxtDataTableCopy
            If Not SaveDataLine() Then
                Return False
            End If
            If Not UpdateIDDB() Then
                Return False
            End If
            TxtDataFirstLine = TxtDataLineStr(0)
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于创建或者更新保存文件，如果表头发生变化，需要及时更新
    Public Function UpdateSaveFile(Optional ByVal tableFirst As String = "") As Boolean
        Try
            If tableFirst = "" Then
                If SaveFilePathStr = "" Then
                    Return False
                Else
                    If Not System.IO.File.Exists(SaveFilePathStr) Then
                        System.IO.File.Create(SaveFilePathStr)
                    End If
                End If
            Else
                If SaveFilePathStr = "" Then
                    Return False
                Else
                    If Not System.IO.File.Exists(SaveFilePathStr) Then
                        System.IO.File.Create(SaveFilePathStr)
                    End If
                    tableFirst = Trim(tableFirst)
                    System.IO.File.WriteAllText(SaveFilePathStr, tableFirst)
                    TxtDataLineStr = System.IO.File.ReadAllLines(SaveFilePathStr)
                End If
            End If
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于删除TxtDataLineStr变量中某一列的所有数据，列数从0开始计数
    Public Function DeleteDBCol(ByVal col As Integer) As Boolean
        Try
            Dim rowIndex As Integer = 0
            Dim rowMaxIndex As Integer = TxtDataLineStr.GetUpperBound(0)
            Dim tempStr As String = ""
            While rowIndex <= rowMaxIndex
                tempStr = TxtDataLineStr(rowIndex)
                Dim tempStrArr() As String = tempStr.Split(" ")
                Dim tempCombineStr As String = ""
                Dim colIndex As Integer = 0
                Dim colMaxIndex As Integer = tempStrArr.GetUpperBound(0)
                While colIndex <= colMaxIndex
                    If colIndex <> col Then
                        tempCombineStr = tempCombineStr & tempStrArr(colIndex) & " "
                    End If
                    '
                    colIndex = colIndex + 1
                End While
                tempCombineStr = tempCombineStr.Substring(0, tempCombineStr.Length - 1)
                TxtDataLineStr(rowIndex) = tempCombineStr
                '
                rowIndex = rowIndex + 1
            End While
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于增加TxtDataLineStr变量中某一列的所有数据，新列输入列名
    Public Function AddDBCol(ByVal colName As String) As Boolean
        Try
            Dim tempStr As String = 0
            Dim rowIndex As Integer = 1
            Dim rowMaxIndex As Integer = TxtDataLineStr.GetUpperBound(0)
            '表头
            TxtDataLineStr(0) = TxtDataLineStr(0) + " " + colName
            '每一行
            While rowIndex <= rowMaxIndex
                TxtDataLineStr(rowIndex) = TxtDataLineStr(rowIndex) + " 0"
                '
                rowIndex = rowIndex + 1
            End While
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
    '
    '' 用于判断某一列是否存在于TxtDataLineStr变量的列名中，列为字符串，返回列的下标
    Public Function IsColumnIn(ByVal colNameStrSet As String, ByVal colNameStr As String) As Integer
        Try
            Dim colNameStrArr() As String = colNameStrSet.Split(" ")
            Dim colIndex As Integer = 0
            Dim colMaxIndex As Integer = colNameStrArr.GetUpperBound(0)
            While colIndex <= colMaxIndex
                If colNameStrArr(colIndex) = colNameStr Then
                    Return colIndex
                End If
                '
                colIndex = colIndex + 1
            End While
            Return -1
        Catch ex As Exception
            Return -1
        End Try
    End Function
    '
    '' 用于按照需要的新的列名更新TxtDataLineStr变量
    Public Function UpdataDBWithNewColName(ByVal colNameStr As String) As Boolean
        Try
            '根据原来表头和现在需要的表头，进行判断，得到更新结果的表头，并创建指示矩阵来表示此列的属性
            Dim oldColNameStr As String = TxtDataLineStr(0)
            Dim newColNameStr As String = Trim(colNameStr)
            Dim oldColNameStrArr() As String = oldColNameStr.Split(" ")
            Dim newColNameStrArr() As String = newColNameStr.Split(" ")
            Dim resColNameGuideArr(100) As String
            Dim resColAttriGuideArr(100) As Integer
            Dim resColNameCount As Integer = 0
            Dim resColNameLen As Integer = 0
            Dim resNewColNameLen As Integer = newColNameStrArr.Length
            '
            Dim trimIterI As Integer = 0
            While trimIterI <= newColNameStrArr.GetUpperBound(0)
                newColNameStrArr(trimIterI) = Trim(newColNameStrArr(trimIterI))
                '
                trimIterI = trimIterI + 1
            End While
            newColNameStr = ""
            trimIterI = 0
            While trimIterI <= newColNameStrArr.GetUpperBound(0)
                newColNameStr = newColNameStr & newColNameStrArr(trimIterI) & " "
                '
                trimIterI = trimIterI + 1
            End While
            newColNameStr = newColNameStr.Substring(0, newColNameStr.Length - 1)
            '
            '新的在旧的里，  旧的里面的下标
            '新的不在旧的里，-1
            '旧的不在新的里，-2
            '
            '处理新的
            resColNameCount = 0
            Dim newIndex As Integer = 0
            While newIndex < newColNameStrArr.Length
                Dim tempStr As String = newColNameStrArr(newIndex)
                Dim resIsColumnIn As Integer = IsColumnIn(oldColNameStr, tempStr)
                If resIsColumnIn <> -1 Then
                    resColAttriGuideArr(resColNameCount) = resIsColumnIn
                Else
                    resColAttriGuideArr(resColNameCount) = -1
                End If
                resColNameGuideArr(resColNameCount) = tempStr
                '
                resColNameCount = resColNameCount + 1
                resColNameLen = resColNameLen + 1
                '
                newIndex = newIndex + 1
            End While
            '
            '处理旧的
            Dim oldIndex As Integer = 0
            While oldIndex < oldColNameStrArr.Length
                Dim tempStr As String = oldColNameStrArr(oldIndex)

                If IsColumnIn(newColNameStr, tempStr) = -1 Then
                    resColAttriGuideArr(resColNameCount) = -2
                    resColNameGuideArr(resColNameCount) = tempStr
                    '
                    resColNameCount = resColNameCount + 1
                    resColNameLen = resColNameLen + 1
                End If
                '
                oldIndex = oldIndex + 1
            End While
            '
            '根据新的表头更新TxtDataLineStr变量
            '新表头
            TxtDataLineStr(0) = newColNameStr
            '每一行
            Dim rowIndex As Integer = 1
            Dim rowMaxIndex As Integer = TxtDataLineStr.GetUpperBound(0)
            While rowIndex <= rowMaxIndex
                Dim tempRowStr As String = TxtDataLineStr(rowIndex)
                Dim tempRowStrArr() As String = tempRowStr.Split(" ")
                Dim tempResRowStr As String = ""
                Dim tempResRowStrArr(resNewColNameLen - 1) As String
                '
                Dim colIndex As Integer = 0
                Dim colMaxIndex As Integer = resNewColNameLen - 1
                While colIndex <= colMaxIndex
                    Dim index As Integer = resColAttriGuideArr(colIndex)
                    If index = -1 Then
                        tempResRowStrArr(colIndex) = "0"
                        tempResRowStr = tempResRowStr & "0 "
                    Else
                        tempResRowStrArr(colIndex) = tempRowStrArr(index)
                        tempResRowStr = tempResRowStr & tempRowStrArr(index) & " "
                    End If
                    '
                    colIndex = colIndex + 1
                End While
                tempResRowStr = tempResRowStr.Substring(0, tempResRowStr.Length - 1)
                TxtDataLineStr(rowIndex) = tempResRowStr
                '
                rowIndex = rowIndex + 1
            End While
            Return True
        Catch ex As Exception
            Return False
        End Try
    End Function
End Class
