import "./index.css"
import * as versesData from './VersesData.ts'

export function App() {

    const firstVerse = versesData.getVerses(0)

    return (
        <>
            {firstVerse.map((verses, index) => (
              <p key={index} className="arabic-text">{verses.arabic}</p>
            ))}
        </>
    )
}

export default App
