import "./index.css";
import verses from './assets/versesimple.json';

interface Data {
    id      : string,
    chapter : string,
    verse   : string,
    english : string,
    arabic  : string,
}

interface Verses {
    type     : string,
    version  : string | null,
    comment  : string | null,
    name     : string | null,
    database : string | null,
    data     : Data[] | null,
}

const parsedVerses = verses as Verses[]
const firstVerse = parsedVerses[2]?.data?.[0]?.arabic

export function App() {
  return (
    <>
        <p className="arabic-text"> {firstVerse} </p>
    </>
  )
}

export default App
