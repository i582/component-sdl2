param (
    [Parameter(Mandatory=$true)][string]$template,
    [Parameter(Mandatory=$true)][string[]]$md,
    [string]$docx,
    [string]$pdf,
    [switch]$embedfonts,
    [switch]$counters
)

if ([string]::IsNullOrEmpty($docx) -and [string]::IsNullOrEmpty($pdf))
{
  Write-error "-docx or -pdf must be specified"
  exit 113
}

$exe = "pandoc.exe"

$template = [System.IO.Path]::GetFullPath($template)

$md = $md | % { [System.IO.Path]::GetFullPath($_ )}

$is_docx_temporary = $False

if ([string]::IsNullOrEmpty($docx))
{
  $docx = [System.IO.Path]::GetTempFilename() + ".docx"
  $is_docx_temporary = $True
}
else
{
  $docx = [System.IO.Path]::GetFullPath($docx)
}

if (-not [string]::IsNullOrEmpty($pdf))
{
  $pdf = [System.IO.Path]::GetFullPath($pdf)
}

$tempdocx = [System.IO.Path]::GetTempFilename() + ".docx"

write-host "Executing Pandoc..."
&$exe $md -o $tempdocx --filter pandoc-crossref --filter pandoc-citeproc --reference-doc $template

if ($LASTEXITCODE -ne 0)
{
  Write-error "Pandoc execution has failed"
  exit 111
}

$word = New-Object -ComObject Word.Application
$curdir = Split-Path $MyInvocation.MyCommand.Path
Set-Location -Path $curdir

$word.ScreenUpdating = $False

$doc = $word.Documents.Open($template)
$doc.Activate()
$selection = $word.Selection

# Save under a new name as soon as possible to prevent auto-saving
# (and polluting) the template
write-host "Saving..."
$doc.SaveAs([ref]$docx)
if (-not $?)
{
  $doc.Close()
  $word.Quit()
  exit 112
}

write-host "Inserting main text..."
if ($selection.Find.Execute("%MAINTEXT%^13", $True, $True, $False, $False, $False, $True, `
       [Microsoft.Office.Interop.Word.wdFindWrap]::wdFindContinue, $False, "",   `
       [Microsoft.Office.Interop.Word.wdReplace]::wdReplaceNone))
{
  $start = $Selection.Range.Start
  $Selection.InsertFile($tempdocx)
  $end = $Selection.Range.End
  $inserted_tables = $doc.Range([ref]$start, [ref]$end).Tables

  # Check if there is anything after the main text
  $selection.WholeStory()
  $totalend = $Selection.Range.End

  # If there is nothing after the main text, remove the extra CR which
  # mystically appears out of nowhere in that case
  if ($end -ge ($totalend - 1))
  {
    $selection.Collapse([Microsoft.Office.Interop.Word.wdCollapseDirection]::wdCollapseEnd)  | out-null
    $selection.MoveLeft([Microsoft.Office.Interop.Word.wdUnits]::wdCharacter, 1, `
                        [Microsoft.Office.Interop.Word.wdMovementType]::wdExtend)  | out-null
    $selection.Delete() | out-null
  }
}

write-host "Inserting number..."
if ($selection.Find.Execute("%NUMBER%^13", $True, $True, $False, $False, $False, $True, `
       [Microsoft.Office.Interop.Word.wdFindWrap]::wdFindContinue, $False, "",   `
       [Microsoft.Office.Interop.Word.wdReplace]::wdReplaceNone))
{
    $selection.TypeText($number)
}


write-host "Inserting title..."
if ($selection.Find.Execute("%TITLE%", $True, $True, $False, $False, $False, $True, `
       [Microsoft.Office.Interop.Word.wdFindWrap]::wdFindContinue, $False, "",   `
       [Microsoft.Office.Interop.Word.wdReplace]::wdReplaceNone))
{
    write-host "find..."
    $selection.TypeText($title)
}



write-host "Searching styles..."
foreach ($style in $doc.Styles)
{
  switch ($style.NameLocal)
  {
    'TableStyleContributors'  {$TableStyleContributors = $style; break}
    'TableStyleAbbreviations' {$TableStyleAbbreviations = $style; break}
    'TableStyleGost'          {$TableStyleGost = $style; break}
    'TableStyleGostNoHeader'  {$TableStyleGostNoHeader = $style; break}
    'UnnumberedHeading1'      {$UnnumberedHeading1 = $style; break}
    'UnnumberedHeading1NoTOC' {$UnnumberedHeading1NoTOC = $style; break}
    'UnnumberedHeading2'      {$UnnumberedHeading2 = $style; break}
  }
}

$BodyText = [Microsoft.Office.Interop.Word.wdBuiltinStyle]::wdStyleBodyText
$Heading1 = [Microsoft.Office.Interop.Word.wdBuiltinStyle]::wdStyleHeading1
$Heading2 = [Microsoft.Office.Interop.Word.wdBuiltinStyle]::wdStyleHeading2
$Heading3 = [Microsoft.Office.Interop.Word.wdBuiltinStyle]::wdStyleHeading3

$bullets = [char]0x2014,[char]0xB0,[char]0x2014,[char]0xB0
$numberposition = 0,0.75,1.75,3
$textposition = 0.85,1.75,3,3.5
$tabposition = 1,1.75,3,3.5
$format_nested = "%1)","%1.%2)","%1.%2.%3)","%1.%2.%3.%4)"
$format_headers = "%1","%1.%2","%1.%2.%3","%1.%2.%3.%4"
$format_single = "%1)","%2)","%3)","%4)"

write-host "Handling list templates..."

foreach ($tt in $doc.ListTemplates)
{
  for ($il = 1; $il -le $tt.ListLevels.Count -and $il -le 4; $il++)
  {
    $level = $tt.ListLevels.Item($il)
    $bullet = ($level.NumberStyle -eq [Microsoft.Office.Interop.Word.wdListNumberStyle]::wdListNumberStyleBullet)
    $arabic = ($level.NumberStyle -eq [Microsoft.Office.Interop.Word.wdListNumberStyle]::wdListNumberStyleArabic)
    $roman  = ($level.NumberStyle -eq [Microsoft.Office.Interop.Word.wdListNumberStyle]::wdListNumberStyleLowercaseRoman)
    
    if ($bullet)
    {
      if ($level.NumberFormat -ne " ")
      {
        $level.NumberFormat = $bullets[$il - 1] + ""
      }
      $level.NumberPosition = $word.CentimetersToPoints($numberposition[$il - 1])
      $level.Alignment = [Microsoft.Office.Interop.Word.wdListLevelAlignment]::wdListLevelAlignLeft
      $level.TextPosition = $word.CentimetersToPoints($textposition[$il - 1])
      $level.TabPosition = $word.CentimetersToPoints($tabposition[$il - 1])
      $level.ResetOnHigher = $il - 1
      $level.StartAt = 1
      $level.Font.Size = 12
      $level.Font.Name = "PT Serif"
      if ($il % 2 -eq 0)
      {
        $level.Font.Position = -4
      }
      $level.LinkedStyle = ""
      $level.TrailingCharacter = [Microsoft.Office.Interop.Word.wdTrailingCharacter]::wdTrailingTab
    }
    
    if (($arabic -and ($level.NumberFormat -ne $format_headers[$il - 1])) -or $roman)
    {
      if ($level.NumberFormat -ne " " )
      {
        if ($arabic)
        {
          $level.NumberFormat = $format_nested[$il - 1]
        }
        if ($roman)
        {
          $level.NumberStyle = [Microsoft.Office.Interop.Word.wdListNumberStyle]::wdListNumberStyleArabic;
          $level.NumberFormat = $format_single[$il - 1]
        }
      }
      $level.NumberPosition = $word.CentimetersToPoints($numberposition[$il - 1])
      $level.Alignment = [Microsoft.Office.Interop.Word.wdListLevelAlignment]::wdListLevelAlignLeft
      $level.TextPosition = $word.CentimetersToPoints($textposition[$il - 1])
      $level.TabPosition = $word.CentimetersToPoints($tabposition[$il - 1])
      $level.ResetOnHigher = $il - 1
      $level.StartAt = 1
      $level.Font.Size = 12
      $level.Font.Name = "PT Serif"
      $level.LinkedStyle = ""
      $level.TrailingCharacter = [Microsoft.Office.Interop.Word.wdTrailingCharacter]::wdTrailingTab
    }
  }
}

# Disable grammar checking (it takes time and spews out error messages)
$doc.GrammarChecked = $True

$ntables = 0

write-host "Handling tables..."

# Loop over other tables
for ($t = 1; $t -le $inserted_tables.Count; $t++)
{
  $table = $inserted_tables.Item($t)
  
  if ($table.Cell(1, 1).Range.Style.NameLocal -eq "ContributorsTable")
  {
    $table.Select()
    $selection.ClearParagraphAllFormatting()
    $pf = $selection.paragraphFormat
    $pf.LeftIndent = 0
    $pf.RightIndent = 0
    $pf.SpaceBefore = 0
    $pf.SpaceBeforeAuto = $False
    $pf.SpaceAfter = 0
    $pf.SpaceAfterAuto = $False

    $table.Style = $TableStyleContributors

    foreach ($row in $table.Rows)
    {
      # Row height can not be set in table style
      $row.Height = $word.CentimetersToPoints(1.4)
      
      # Alignment and line spacing are set in table style, but are not applied (low priority?)
      # So we set them explicitly
      $row.Select()
      $pf = $selection.paragraphFormat
      $pf.Alignment = [Microsoft.Office.Interop.Word.wdParagraphAlignment]::wdAlignParagraphLeft
      $pf.LineSpacingRule = [Microsoft.Office.Interop.Word.wdLineSpacing]::wdLineSpaceSingle
    }
    
    continue
  }
  
  if ($table.Cell(1, 1).Range.Style.NameLocal -eq "AbbreviationsTable")
  {
    $table.Style = $TableStyleAbbreviations
    $table.Select()
    $pf = $selection.paragraphFormat
    $selection.ClearParagraphAllFormatting()
    $pf.LeftIndent = 0
    $pf.RightIndent = 0
    $pf.SpaceBefore = 0
    $pf.SpaceBeforeAuto = $False
    $pf.SpaceAfter = 0
    $pf.SpaceAfterAuto = $False
    continue
  }
  
  # This is to fix the widths of the columns
  $table.AllowAutoFit = $False
	  
  # Numbered equations are 2-column tables without titles and borders, and with equations in both columns
  if ([string]::IsNullOrEmpty($table.Title) `
    -and (-not $table.Rows.Item(1).Cells.Borders.Item([Microsoft.Office.Interop.Word.wdBorderType]::wdBorderBottom).Visible) `
	  -and ($table.Columns.Count -eq 2) `
	  -and ($table.Cell(1,1).Range.OMaths.Count -eq 1) `
	  -and ($table.Cell(1,2).Range.OMaths.Count -eq 1))
  {
    # There can be multiple equations (rows) in one table
    foreach ($row in $table.Rows)
    {
      # After removing the equation, the text contents remains
      if ($row.Cells.Item(2).Range.OMaths.Count -ne 0)
      {
        $row.Cells.Item(2).Range.OMaths.Item(1).Remove()
      }
                
      $row.Cells.Item(2).VerticalAlignment = [Microsoft.Office.Interop.Word.wdCellVerticalAlignment]::wdCellAlignVerticalCenter;
      $row.Cells.Item(2).Select()
      $selection.ClearParagraphAllFormatting()
      $pf = $selection.paragraphFormat
      $pf.LeftIndent = $word.CentimetersToPoints(0)
      $pf.RightIndent = $word.CentimetersToPoints(0)
      $pf.Alignment = [Microsoft.Office.Interop.Word.wdParagraphAlignment]::wdAlignParagraphRight
      $pf.SpaceBefore = 0
      $pf.SpaceBeforeAuto = $False
      $pf.SpaceAfter = 0
      $pf.SpaceAfterAuto = $False
      $pf.LineSpacingRule = [Microsoft.Office.Interop.Word.wdLineSpacing]::wdLineSpaceSingle
      $pf.CharacterUnitLeftIndent = 0
      $pf.CharacterUnitRightIndent = 0
      $pf.LineUnitBefore = 0
      $pf.LineUnitAfter = 0
    }
  }
  else # Ordinary tables
  {
    # Count only tables with title (and number)
    if (-not [string]::IsNullOrEmpty($table.Title))
    {
      $ntables = $ntables + 1
    }
    
    $table.Select()
    
    $pf = $selection.paragraphFormat
    $pf.LineSpacingRule = [Microsoft.Office.Interop.Word.wdLineSpacing]::wdLineSpaceSingle
    
    # If the first row has line under it, then it is a table with a header row
    if ($table.Rows.Item(1).Cells.Borders.Item([Microsoft.Office.Interop.Word.wdBorderType]::wdBorderBottom).Visible)
    {
      $table.Style = $TableStyleGost
    }
    else # table without header row
    {
      $table.Style = $TableStyleGostNoHeader
    }
    
    # Fix alignment of display-math objects so they math the aligment of text in the cells
    foreach ($row in $table.Rows)
    {
      foreach ($cell in $row.Cells)
      {
        foreach ($math in $cell.Range.OMaths)
        {
          if ($math.Type -eq [Microsoft.Office.Interop.Word.wdOMathType]::wdOMathDisplay)
          {
            $al = $cell.Range.ParagraphFormat.Alignment
            if ($al -eq [Microsoft.Office.Interop.Word.wdParagraphAlignment]::wdAlignParagraphRight)
            {
              $math.Justification = [Microsoft.Office.Interop.Word.wdOMathJc]::wdOMathJcRight;
            }
            elseif ($al -eq [Microsoft.Office.Interop.Word.wdParagraphAlignment]::wdAlignParagraphLeft)
            {
              $math.Justification = [Microsoft.Office.Interop.Word.wdOMathJc]::wdOMathJcLeft;
            }
            elseif ($al -eq [Microsoft.Office.Interop.Word.wdParagraphAlignment]::wdAlignParagraphCenter)
            {
              $math.Justification = [Microsoft.Office.Interop.Word.wdOMathJc]::wdOMathJcCenter;
            }
          }
        }
      }
    }
  }
}

write-host "Updating paragraph styles..."

$heading1namelocal = $doc.styles.Item([Microsoft.Office.Interop.Word.wdBuiltinStyle]::wdStyleHeading1).NameLocal
$nchapters = 0
$nfigures = 0
$nreferences = 0
$nappendices = 0

foreach ($par in $doc.Paragraphs)
{
  $namelocal = $par.Range.CharacterStyle.NameLocal
  if ($namelocal -eq "UnnumberedHeadingOne")
  {
    $par.Style = $UnnumberedHeading1
  }
  elseif ($namelocal -eq "AppendixHeadingOne")
  {
    $par.Style = $UnnumberedHeading1
    $nappendices = $nappendices + 1
  }
  elseif ($namelocal -eq "UnnumberedHeadingOneNoTOC")
  {
    $par.Style = $UnnumberedHeading1NoTOC
  }
  elseif ($namelocal -eq "UnnumberedHeadingTwo")
  {
    $par.Style = $UnnumberedHeading2
  }
  else
  {
    $namelocal = $par.Style.NameLocal
    # Make source core paragraphs smaller
    if ($namelocal -eq "Source Code")
    {
      $par.Range.Font.Size = 10.5
    }
    # No special style for first paragraph to avoid unwanted space
    # between first and second paragraphs
    elseif ($namelocal -eq "First Paragraph")
    {
      $par.Style = $BodyText
    }
    elseif ($namelocal -eq $heading1namelocal)
    {
      $nchapters = $nchapters + 1
    }
    elseif ($namelocal -eq "Captioned Figure")
    {
      $nfigures = $nfigures + 1
    }
    elseif ($namelocal -eq "ReferenceItem")
    {
      $nreferences = $nreferences + 1
    }
  }
}

if ($counters)
{
  write-host "Inserting number of chapters, figures, and tables..."
  $selection.HomeKey([Microsoft.Office.Interop.Word.wdUnits]::wdStory) | out-null
  $selection.Find.Execute("%NCHAPTERS%", $True, $True, $False, $False, $False, $True, `
         [Microsoft.Office.Interop.Word.wdFindWrap]::wdFindContinue, $False, $nchapters + "",   `
         [Microsoft.Office.Interop.Word.wdReplace]::wdReplaceOne) | out-null
  $selection.HomeKey([Microsoft.Office.Interop.Word.wdUnits]::wdStory) | out-null
  $selection.Find.Execute("%NFIGURES%", $True, $True, $False, $False, $False, $True, `
         [Microsoft.Office.Interop.Word.wdFindWrap]::wdFindContinue, $False, $nfigures + "",   `
         [Microsoft.Office.Interop.Word.wdReplace]::wdReplaceOne) | out-null
  $selection.HomeKey([Microsoft.Office.Interop.Word.wdUnits]::wdStory) | out-null
  $selection.Find.Execute("%NTABLES%", $True, $True, $False, $False, $False, $True, `
         [Microsoft.Office.Interop.Word.wdFindWrap]::wdFindContinue, $False, $ntables + "",   `
         [Microsoft.Office.Interop.Word.wdReplace]::wdReplaceOne) | out-null
  $selection.HomeKey([Microsoft.Office.Interop.Word.wdUnits]::wdStory) | out-null
  $selection.Find.Execute("%NREFERENCES%", $True, $True, $False, $False, $False, $True, `
         [Microsoft.Office.Interop.Word.wdFindWrap]::wdFindContinue, $False, $nreferences + "",   `
         [Microsoft.Office.Interop.Word.wdReplace]::wdReplaceOne) | out-null
  $selection.HomeKey([Microsoft.Office.Interop.Word.wdUnits]::wdStory) | out-null
  $selection.Find.Execute("%NAPPENDICES%", $True, $True, $False, $False, $False, $True, `
         [Microsoft.Office.Interop.Word.wdFindWrap]::wdFindContinue, $False, $nappendices + "",   `
         [Microsoft.Office.Interop.Word.wdReplace]::wdReplaceOne) | out-null
}
       
write-host "Increasing math font size..."

foreach ($math in $doc.OMaths)
{
  # Size equations up a bit to match Paratype font size
  $math.Range.Font.Size = 12.5
}

write-host "Handling INCLUDEs..."
$selection.HomeKey([Microsoft.Office.Interop.Word.wdUnits]::wdStory) | out-null

while ($selection.Find.Execute("%INCLUDE(*)%^13", $True, $True, $True, $False, $False, $True, `
       [Microsoft.Office.Interop.Word.wdFindWrap]::wdFindContinue, $False, "", `
       [Microsoft.Office.Interop.Word.wdReplace]::wdReplaceNone))
{
  if ($selection.Text -match '%INCLUDE\((.*)\)%')
  {
    $filename = $matches[1]
    
    $start = $Selection.Range.Start
    $Selection.InsertFile([System.IO.Path]::GetFullPath($filename))
    
    if (!$?)
    {
      break
    }
      
    $end = $Selection.Range.End

    # Check if there is anything after the inserted documnt
    $selection.WholeStory()
    $totalend = $Selection.Range.End

    # If there is nothing after the inserted documnt, remove the extra CR which
    # mystically appears out of nowhere in that case
    if ($end -ge ($totalend - 1))
    {
      $selection.Collapse([Microsoft.Office.Interop.Word.wdCollapseDirection]::wdCollapseEnd)  | out-null
      $selection.MoveLeft([Microsoft.Office.Interop.Word.wdUnits]::wdCharacter, 1, `
                          [Microsoft.Office.Interop.Word.wdMovementType]::wdExtend)  | out-null
      $selection.Delete() | out-null
    }
  }
}

$selection.HomeKey([Microsoft.Office.Interop.Word.wdUnits]::wdStory) | out-null

write-host "Inserting ToC..."
if ($selection.Find.Execute("%TOC%^13", $True, $True, $False, $False, $False, $True, `
       [Microsoft.Office.Interop.Word.wdFindWrap]::wdFindContinue, $False, "",   `
       [Microsoft.Office.Interop.Word.wdReplace]::wdReplaceNone))
{
  $doc.TablesOfContents.Add($selection.Range, $False, 9, 9, $False, "", $True, $True, "", $True) | out-null
  
  # Manually add level 1,2,3 headers to ToC
  $toc = $doc.TablesOfContents.Item(1)
  $toc.UseHeadingStyles = $True
  $toc.HeadingStyles.Add($UnnumberedHeading1, 1) | out-null
  $toc.HeadingStyles.Add($UnnumberedHeading2, 2) | out-null
  $toc.HeadingStyles.Add($Heading1, 1) | out-null
  $toc.HeadingStyles.Add($Heading2, 2) | out-null
  $toc.HeadingStyles.Add($Heading3, 3) | out-null
  $toc.Update() | out-null
}

write-host "Inserting number of pages..."

# Seemingly is not needed but who knows
$doc.Repaginate()

# Inserting "section pages" field gets the number of pages wrong, and no way has
# been found to remedy that other than manual update in Word.
# So here is another way to get the number of pages in the section

if ($doc.Sections.Count -gt 1) # two-section template?
{
  $npages = $doc.Sections.Item(2).Range.Information([Microsoft.Office.Interop.Word.wdInformation]::wdActiveEndPageNumber) - `
            $doc.Sections.Item(1).Range.Information([Microsoft.Office.Interop.Word.wdInformation]::wdActiveEndPageNumber)
}
else
{
  $npages = $doc.Sections.Item(1).Range.Information([Microsoft.Office.Interop.Word.wdInformation]::wdNumberOfPagesInDocument)
}

$selection.HomeKey([Microsoft.Office.Interop.Word.wdUnits]::wdStory) | out-null
$selection.Find.Execute("%NPAGES%", $True, $True, $False, $False, $False, $True, `
         [Microsoft.Office.Interop.Word.wdFindWrap]::wdFindContinue, $False, $npages + "",   `
         [Microsoft.Office.Interop.Word.wdReplace]::wdReplaceOne) | out-null

if ($embedfonts)
{
  # Embed fonts (for users who do not have Paratype fonts installed).
  # This costs a few MB in file size
  $word.ActiveDocument.EmbedTrueTypeFonts = $True
  $word.ActiveDocument.DoNotEmbedSystemFonts = $True
  $word.ActiveDocument.SaveSubsetFonts = $True 
}

if (-not $is_docx_temporary)
{
  write-host "Saving docx..."
  $doc.Save()
}

if (-not [string]::IsNullOrEmpty($pdf))
{
  write-host "Saving PDF..."
  $doc.SaveAs2([ref]$pdf, [ref][Microsoft.Office.Interop.Word.wdSaveFormat]::wdFormatPDF)
}

$doc.Close()
$word.Quit()

write-host "Removing temporary files..."
Remove-item -path $tempdocx
if ($is_docx_temporary)
{
  Remove-item -path $docx
}
