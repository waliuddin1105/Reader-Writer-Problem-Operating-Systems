<h1 align="center">Readers-Writers Problem Simulation using POSIX Threads and Semaphores (C)</h1>
<p align="center">
  <b>Implementation of the classic Readers–Writers synchronization problem using pthreads and semaphores in C.</b>
</p>
<hr>
<h2>📘 Overview</h2>
<p>
This project implements the <b>Readers–Writers Problem</b> using <b>POSIX threads (pthreads)</b> and <b>semaphores</b> in the C programming language. It demonstrates how concurrent access to a shared resource can be safely managed by multiple reader and writer threads while maintaining data consistency and synchronization.
</p>
<hr>
<h2>⚙️ Features</h2>
<ul>
  <li>Uses <b>pthread</b> and <b>semaphore</b> libraries for synchronization.</li>
  <li>Prevents <b>race conditions</b> in shared resource access.</li>
  <li>Handles multiple readers and writers efficiently.</li>
  <li>Logs each thread’s request, entry, and exit times.</li>
  <li>Calculates and displays the <b>average waiting time</b> for all threads.</li>
</ul>
<hr>
<h2>🧩 Technical Details</h2>
<ul>
  <li><b>Language:</b> C</li>
  <li><b>Libraries Used:</b> pthread, semaphore</li>
  <li><b>Platform:</b> Linux (tested on Ubuntu)</li>
</ul>
<hr>
<h2>🚀 Compilation & Execution</h2>
<pre><code># Compile
gcc readers_writers.c -lpthread -o readers_writers

# Run
./readers_writers
</code></pre>
<hr>
<h2>📂 Input Format</h2>
<p>The program reads the number of readers and writers from an input file. Example:</p>
<pre><code>3 2
</code></pre>
<p>This means there will be 3 reader threads and 2 writer threads.</p>
<hr>
<h2>📊 Output Example</h2>
<pre><code>Reader 1 requesting to read
Reader 1 entered critical section
Reader 2 requesting to read
Writer 1 requesting to write
Reader 1 exiting critical section
Writer 1 entered critical section
Writer 1 exiting critical section

Average waiting time: 0.0023 seconds
</code></pre>
<hr>
<h2>🏷️ Topics</h2>
<p>
<code>operating-systems</code> • 
<code>pthread</code> • 
<code>semaphores</code> • 
<code>reader-writer-problem</code> • 
<code>concurrency</code> • 
<code>synchronization</code> • 
<code>multithreading</code> • 
<code>c-programming</code> • 
<code>critical-section</code> • 
<code>posix-threads</code>
</p>
