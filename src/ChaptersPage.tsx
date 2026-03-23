import "./index.css"
import { useSearchParams, Link } from 'react-router-dom';
import * as versesData from './VersesData.ts'

export function ChaptersPage() {
  const [searchParams] = useSearchParams();
  const chapter = searchParams.get('chapter');
  const verses = versesData.getVerses(Number(chapter) - 1);

  if (!verses) return <p>Not found</p>;
    return (
        <>
            {<p>
                <Link to={`/`}> Home </Link>
            </p>}

            {verses.map((verse, index) => (
              <p key={index} className="arabic-text">{verse.arabic}</p>
            ))}
        </>
    )
}
