%% 
%% eitExjobb.cls
%% By: Stefan Höst, stefan.host@eit.lth.se
%%
%%%%%%%%%%%%%%%%
%% 
%% 2009-12-04
%% Fixed fonts for LoF and LoT
%%
%%%%%%%%%%%%%%%% 
%% This is a class to be used for Master's thesises at the department of
%% Electrical and Information Technology at Lund University,
%% Sweden. It is based on the standard class report.cls. I have tried
%% to make comments so it is easy to find where to make desired
%% changes. 
%% 
%% IMPORTANT!!
%% To avoid confusion: 
%% If you do make changes then also change the name of the file!!! 
%% If I would make changes in the file I would call it e.g. 
%%   SHeitExjobb.cls 
%% Then I must also change the second line in the code to 
%%   \ProvidesClass{eitExjobb}
%% Otherwise LaTeX will give a warning. Put the new file in the same
%% directory as the report and call the new file with 
%%   \documentclass[...]{eitExjobb}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{eitExjobb}
%% Boolean variables used by the class. The standard way I use is to
%% specify a Boolean variable for each option. Then I can use if-else
%% statements in the definitions later.
\newif\ifshfinal\shfinalfalse
%% Options. If final is chosen it rearanges the layout so it can be
%% printed by PH and Johan. Default is to print the text in the middle
%% of the page and then put a frame round it.
\DeclareOption{final}{\shfinaltrue}
%% Process options
\ProcessOptions\relax
%% Load everything in report.cls
\LoadClassWithOptions{report}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Geometry. A package for page layout.
\RequirePackage{geometry}
%%%%%%%%%%%%%%%%%%%%%
%% graphicx (or graphics)
\RequirePackage{graphicx}
% %% colors
% \RequirePackage{color}
% %%  grids
% \RequirePackage{graphpap}
%%%%%%%%%%%%%%%%%%%%%
%% Option final?
\ifshfinal %% Final. Print as book with left side 1in.
  \geometry{paper=a4paper,
    body={125mm,200mm},
    left=29.5mm,
    vmarginratio=1:1}
\else %% Not final. Put frame round the page.
  \geometry{papersize={169mm,239mm},%% G5
    body={125mm,200mm},
    left=29.5mm,
    vmarginratio=1:1}
  %% Crop marks (the frame; can also be set to cam and some others)
  \RequirePackage[frame,a4,dvips,center]{crop}
\fi
%%%%%%%%%%%%%%%%%%%%%
%% How to write standard counters.
\def\figurename{Figure}
\def\thefigure{\thechapter.\arabic{figure}}
\def\tablename{Table}
\def\thetable{\thechapter.\arabic{table}}
%%%%%%%%%%%%%%%%%%%%%
%% Captions. A package that controlls how the captions (figure and
%% table) are printed.
\RequirePackage{caption}
\captionsetup{margin=10pt,%
  indention=8mm,%
  font={sf,small},%
  labelfont=bf,%
  width=.8\textwidth}
%%\captionsetup[table]{position=above}
%%%%%%%%%%%%%%%%%%%%% 
%% Headers and footer. 
\RequirePackage{fancyhdr}
\fancyhead{}
\fancyfoot{}
\pagestyle{fancy}
%% See to that \rightmark is always the chapter name.
\def\chaptermark#1{\markright{#1}} 
\def\sectionmark#1{}
%% Page numbering before 'Introduction'. Only small roman numbers at
%% the end of the page.
\def\frontmatter{%
  \def\headrulewidth{0pt}
  \def\thepage{\roman{page}}
  \fancyfoot[CO,CE]{\thepage}
}
%% Page numbering in the main document. The footer is empty and the
%% header has the page number and the chapter name. 
\def\mainmatter{%
  \setcounter{page}{1}
  \def\thepage{\arabic{page}}
  \fancyfoot{}
  \fancyhead[LE,RO]{\small\textsf{\thepage}}
  \fancyhead[LO,RE]{\small\textsf{\rightmark}}
  \def\headrulewidth{0.4pt}
}
%%%%%%%%%%%%%%%%%%%%%
%% Font size changing package
\RequirePackage{relsize}
%%%%%%%%%%%%%%%%%%%%%
%% Section style
\RequirePackage{sectsty}
%% Make all sections sans serif and different sizes.
\sectionfont{\normalsize\relsize{2}\sffamily\mdseries}
\subsectionfont{\normalsize\relsize{1}\sffamily\mdseries}
\subsubsectionfont{\normalsize\relsize{0}\sffamily\mdseries}
%%%%%
%% Chapters style. This was a bit wors. I have redifined the commands
%% \@makechapterhead and \@makeschapterhead from report.cls. The first
%% one controlls the printing of a chapter with numbers and the second
%% without numbers. Before the actual definitions I have some small
%% fixes. First I put a \cleardoublepage in front of chapter to always
%% get a new chapter on an odd (right) side. Then I need a little bit
%% thicker \hrulefill (just redefine  \hrulefill), and then sets
%% \chaptername and \chapterfont. The latter is the font for the
%% chapter name. There should also be font commands for \chaptername
%% and chapter numbering. But tis is for future work
\let\sh@chapter\chapter
\def\chapter{\cleardoublepage\sh@chapter}
\def\hrulefillchap{\leavevmode\leaders\hrule height 1.0pt \hfill\kern\z@}
\def\chaptername{Chapter}
\def\chapterfont{\fontsize{40}{45}\selectfont}
%% chapter
\def\@makechapterhead#1{%
  \vspace*{4\p@}%
  {\parindent \z@ \raggedright \normalfont
    \ifnum \c@secnumdepth >\m@ne
      \normalsize\sffamily\mdseries
      \hrulefillchap\space\@chapapp\space\chapterfont\thechapter%
      \par\nobreak
      \vskip 5\p@
    \fi
    \interlinepenalty\@M
    \normalsize\sffamily\mdseries
    \raggedleft\relsize{4}
    #1
    \par\nobreak
    \vskip -7\p@
    \hrulefillchap\par\nobreak
    \vskip 40\p@
  }}
%% chapter*
\def\@makeschapterhead#1{%
  \vspace*{40\p@}%
  {\parindent \z@ \raggedright
    \normalfont
    \hrulefillchap
    \par\nobreak
    \vskip 5\p@
    %%
    \interlinepenalty\@M
    \normalsize\sffamily\mdseries
    \raggedleft\relsize{4}
    #1
    \par\nobreak
    \vskip -7\p@
    \hrulefillchap\par\nobreak
    \vskip 40\p@
  }}
%%%%%%%%%%%%%%%%%%%%%
%% Toc style. First I just set. Import the
%% tocloft package and set the title of the ToC, LoF and LoT. I want
%% only chapters and sections in the ToC, so I set tocdepth to 1
%% (0=chapter, 1=sections, 2=subsections,...). The redifintion of
%% \@cftmaketoctitle is done since the tocloft package only prints the
%% title with a certain font, and like it as achapter (without numbers). 
\RequirePackage{tocloft}
\setcounter{tocdepth}{2}
\def\contentsname{Table of Contents}
\def\listfigurename{List of Figures}
\def\listtablename{List of Tables}
%% Title defs
\def\@cftmaketoctitle{%
  \addpenalty\@secpenalty
  \@cftpagestyle
  {\interlinepenalty\@M
    \chapter*{\contentsname}
    \cftmarktoc
  }}
\def\@cftmakeloftitle{%
 \addpenalty\@secpenalty
 \@cftpagestyle
 {\interlinepenalty\@M
   \chapter*{\listfigurename}
   \cftmarklof
 }}
\def\@cftmakelottitle{%
 \addpenalty\@secpenalty
 \@cftpagestyle
 {\interlinepenalty\@M
   \chapter*{\listtablename}
   \cftmarklot
 }}
%% Get the right numbering
\tocloftpagestyle{fancy}
%% Now follows specifications for how to print the ToC.
%% chapter
\def\cftchapfont{\sffamily\bfseries}
\def\cftchappagefont{\sffamily\bfseries}
\def\cftchapleader{\hrulefill}
%% section
\def\cftsecfont{\sffamily\mdseries}
\def\cftsecpagefont{\sffamily\mdseries}
%% subsection
\def\cftsubsecfont{\sffamily\mdseries\relsize{-1}}
\def\cftsubsecpagefont{\sffamily\mdseries\relsize{-1}}
\def\cftsubsecleader{\hfill}
%% subsubsection
\def\cftsubsubsecfont{\sffamily\mdseries\relsize{-2}}
\def\cftsubsubsecpagefont{\sffamily\mdseries\relsize{-2}}
\def\cftsubsubsecleader{\hfill}
%% LoF
\def\cftfigfont{\sffamily\mdseries}
\def\cftfigpagefont{\sffamily\mdseries}
%% LoT
\def\cfttabfont{\sffamily\mdseries}
\def\cfttabpagefont{\sffamily\mdseries}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Bibliography. Set the title of bibliography in \bibname. Add to
%% \thebibliography environment that the header looks as before and
%% that it appears in the ToC. (To remove it from the ToC just
%% redefine \thebibliography exactly as below but without the last row.
\def\bibname{References}
\let\sh@thebibliography\thebibliography
\def\thebibliography#1{%
  \sh@thebibliography{#1}
  \markright{\bibname}
  \addcontentsline{toc}{chapter}{\bibname}
}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Definitions for the Titlepage
\def\Title#1{\def\sh@Title{#1}}
\def\Author#1{\def\sh@Author{#1}}
\def\Date#1{\def\sh@Date{#1}}
\def\Advisor#1{\def\sh@Advisor{#1}}
\def\Year#1{\def\sh@Year{#1}}
\def\Company#1{\def\sh@Company{#1}}
\Title{}
\Author{}
\Date{\today}
\Advisor{}
\Year{\the\year} %% this year
\Company{Department of Electrical and Information Technology\\Lund University}
%%
\def\MakeTitlePage{%
  \thispagestyle{empty}
  \strut\vfill
  \begin{center}
    \LARGE \sh@Title\\[2\baselineskip]
    \large \sh@Author\\[\baselineskip]
    \large \sh@Company\\[2\baselineskip]
    Advisor: \sh@Advisor\\[\baselineskip]
    \sh@Date
  \end{center}
  \vfill\vfill\strut
  \newpage
  %% copyright
  \thispagestyle{empty}
  \strut\vfill
  \small\noindent

  \noindent\strut\vspace*{\baselineskip}\newline
  Printed in Sweden\newline
  E-huset, Lund, \sh@Year
  %%
  \clearpage
  \setcounter{page}{1}
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\endinput
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%