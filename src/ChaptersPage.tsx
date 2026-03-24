import "./index.css"
import { useSearchParams, Link } from 'react-router-dom'
import * as versesData from './VersesData.ts'


const versePerpageCount: number = 50

export function ChaptersPage() {
    const [searchParams] = useSearchParams()
    const chapter = Number(searchParams.get('chapter'))
    const page = Number(searchParams.get('page'))

    const verseCount = versesData.surahsMetadata[chapter - 1].versesCount

    const verses = versesData.getVerses(chapter - 1)
    if (!verses) return <p>Not found</p>;

    const pageCount = getPageCount(verseCount)
    if ( page > pageCount) return <p>Not found</p>

    const pageIndex = getPageIndex(chapter,page,pageCount,verseCount)
    const pageLinks = getPageLinks(chapter,pageCount)


    return (
        <>
            {<p>
                <Link to={`/`}> Home </Link>
            </p>}

            {verses.slice(pageIndex.start,pageIndex.end).map((verse, index) => (
                <p key={index} className="arabic-text">{verse.arabic}</p>
            ))}

            {<nav>{pageLinks}</nav>}
        </>
    )
}


const getPageCount =  (verseCount) => {
    let count : number = 1

    if (verseCount > versePerpageCount) {
        const quotient = Math.floor(verseCount / versePerpageCount)
        const modulo = verseCount % versePerpageCount
        count = quotient + Number(modulo != 0)
    }

    return count
}


const getPageIndex = (chapter,page,pageCount,verseCount) => {
  const surahNumber =  versesData.surahsMetadata[chapter - 1].number
  let start : number = 0
  let end : number = start + verseCount + 1 * Number(surahNumber != 1 || surahNumber != 9)

    if (pageCount > 1) {
        start = versePerpageCount * (page - 1)
        if (page != pageCount) // if not in last page
            end = start + versePerpageCount
    }

  return {
    start : start,
    end   : end,
  }
}

const getPageLinks = (chapter,pageCount) => {
    const links = []
    for (let i = 1; i <= pageCount; i++) {
        links.push(<Link
                      key={i}
                      to={`?chapter=${chapter}&page=${i}`}
                      onClick={() => window.scrollTo(0, 0)}>
                    | {i} |
                   </Link>)
    }

    return links
}
