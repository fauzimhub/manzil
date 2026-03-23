import "./index.css"
import { useSearchParams, Link } from 'react-router-dom'
import * as versesData from './VersesData.ts'

export function ChaptersPage() {
  const [searchParams] = useSearchParams()
  const chapter = searchParams.get('chapter')
  const page = searchParams.get('page')
  const verses = versesData.getVerses(Number(chapter) - 1)
  if (!verses) return <p>Not found</p>;

  const versePerpageCount: number = 50
  const verseCount: number = versesData.surahsMetadata[Number(chapter) - 1].versesCount
  const surahNumber: number = versesData.surahsMetadata[Number(chapter) - 1].number
  let pageCount : number = 1

    if (verseCount > versePerpageCount) {
        const quotient = verseCount / versePerpageCount
        const modulo = verseCount % versePerpageCount
        pageCount = quotient + Number(modulo != 0)
    }
  if ( Number(page) > pageCount) return <p>Not found</p>

  let pageStartIndex : number = 0
  let pageEndIndex : number = pageStartIndex + verseCount - 1 * Number(surahNumber == 1 || surahNumber == 9)

    if (pageCount > 1) {
      pageStartIndex = versePerpageCount * (Number(page) - 1)
      if (Number(page) != pageCount) // if not in last page
        pageEndIndex = pageStartIndex + versePerpageCount
    }

    const pages = []
    for (let i = 1; i <= pageCount; i++) {
        pages.push(<Link
                      key={i}
                      to={`?chapter=${chapter}&page=${i}`}
                      onClick={() => window.scrollTo(0, 0)}>
                    | {i} |
                   </Link>)
    }

    return (
        <>
            {<p>
                <Link to={`/`}> Home </Link>
            </p>}

            {verses.slice(pageStartIndex,pageEndIndex).map((verse, index) => (
                <p key={index} className="arabic-text">{verse.arabic}</p>
            ))}

            {<nav>{pages}</nav>}
        </>
    )
}
