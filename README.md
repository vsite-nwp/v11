#### vježba 11

Koristeći MFC napravite aplikaciju koja koristi Fluent UI. Mišem se nacrta pravokutnik, a na ribbonu se bira lik koji se iscrtava unutar pravokutnika i njegova boja.

Oblik i boja lika se mijenjaju kada miš prelazi preko odgovarajućeg polja (*preview*).

![frame](/screenshot.jpg?raw=true)


Uputstva:

APLIKACIJA
*	napravite pomoću AppWizard-a MFC aplikaciju:
  * Application type = "Single document"
  * "Project style" = "Office"
  * "Command bars" = "Use a ribbon"
  * “Advanced features” i “Advanced frame panes” maknite sve
*	------------------------------------------------------------------------------------
*	u View klasu dodajte članove tipa CRect (`rc`), COLORREF (`color`) i int (`shape`)
*	u konstruktoru View klase inicijalizirate `shape` i `color` na 0
*	u OnDraw napravite i selektirajte pen odgovarajuće boje i nacrtajte odgovarajući lik (shape: 0 = Rectangle, 1 = Ellipse, 2 = RoundRect)
*	dodajte handler za `WM_LBUTTONDOWN`, u njemu koristeći `CRectTracker` objekt i metodu `TrackRubberBand()` dohvatite pravokutnik (član `m_rect`) i invalidirajte prozor

RIBBON
*	napravite 24-bitnu bitmapu (koriteći Paint) koja ima 3 slike sa 3 lika (recimo 120 x 40) i spremite je (res/shapes.bmp)
*	dodajte bitmapu u resurse (desno dugme na resurse, „Add Resource...“, pa „Import... “), id =  IDB_SHAPES
*	otvorite Ribbon Designer (double-click na ribbon u resursima)
*	dodajte kategoriju (Caption=„Shape“) i postavite joj slike (Large Images=IDB_BUTTONS)
*	na panel dodajte Gallery Button i postavite mu slike (ID=ID_SHAPE, Icons=IDB_SHAPES, Icon Width=40, Large Image Index=0)
*	na panel dodajte Color Button (ID=ID_COLOR, Caption=“Color“, Other Button/Caption=“more colors...“)
*	prebacite član CMainFrame::m_wndRibbonBar u public
*	------------------------------------------------------------------------------------
*	dodajte event-handler za oba buttona (u View klasu), oba invalidiraju prozor
*	za lik - koristeći `CMFCRibbonBar::GetElementsByID` dođite do `CMFCRibbonGallery` objekta, metoda `GetSelectedItem()` vraća indeks odabranog polja:

	```
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_SHAPE, arr);
	CMFCRibbonGallery* pGallery = (CMFCRibbonGallery*)arr.GetAt(0);
	```

*	za boju - koristite `CMFCRibbonColorButton` objekt i metodu `GetColor()`

PREVIEW
*	u CMainFrame odgovorite na registriranu poruku `AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM` i proslijedite poruku aktivnom prozoru (`GetActiveView`)
*	u `View` klasi odgovorite na istu poruku
```
LRESULT Cv11View::OnHighlightRibbonListItem(WPARAM wp, LPARAM lp)
{
		int index = (int) wp;
		CMFCRibbonBaseElement* pElem = (CMFCRibbonBaseElement*) lp;
		UINT id = pElem->GetID(); // button id (ID_SHAPE, ID_COLOR)
		// ...
}
```
*	sačuvajte trenutno stanje, promijenite vrijednosti članova i invalidirajte prozor - za lik koristite index, a za boju objekt `CMFCRibbonColorButton` i metodu `GetHighlightedColor()`
*	za index == -1 vratite sačuvano stanje i invalidirajte prozor

