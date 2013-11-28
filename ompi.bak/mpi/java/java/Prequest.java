/*
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
/*
 * File         : Prequest.java
 * Author       : Sang Lim, Xinying Li, Bryan Carpenter
 * Created      : Thu Apr  9 12:22:15 1998
 * Revision     : $Revision: 1.11 $
 * Updated      : $Date: 2001/10/22 21:07:55 $
 * Copyright: Northeast Parallel Architectures Center
 *            at Syracuse University 1998
 */


/*
 * Note: in the end there is no sensible way to use the native
 * persistent requests here.  For every `start'/`wait' cycle you need
 * to do `get...Elements', `release...Elements', otherwise the behaviour
 * is wrong if pinning isn't supported (because then get/release ops
 * need active copying to move values between C and Java).
 *
 * (Even if pinning is supported, the arrays would have to be pinned
 * almost permanently, which presumably isn't a good thing.)
 */

package mpi;

/**
 * Persistent request object.
 */
public final class Prequest extends Request
{
protected final static int MODE_STANDARD    = 0;
protected final static int MODE_BUFFERED    = 1;
protected final static int MODE_SYNCHRONOUS = 2;
protected final static int MODE_READY       = 3;

private int mode, offset, count, src, dest, tag;
private Object buf;
private Datatype type;
private Comm comm;

/**
 * Constructor used by {@code sendInit}, etc.
 */
protected Prequest(long handle)
{
    super(handle);
}

/**
 * Activate a persistent communication request.
 * <p>Java binding of the MPI operation {@code MPI_START}.
 * The communication is completed by using the request in
 * one of the {@code wait} or {@code test} operations.
 * On successful completion the request becomes inactive again.
 * It can be reactivated by a further call to {@code Start}.
 */
public void start() throws MPIException
{
    start_jni();
}

private native void start_jni() throws MPIException;

/**
 * Activate a list of communication requests.
 * <p>Java binding of the MPI operation {@code MPI_STARTALL}.
 * @param requests array of requests
 * @throws MPIException 
 */
public static void startAll(Prequest[] requests) throws MPIException
{
    MPI.check();
    startAll_jni(requests);
}

private native static void startAll_jni(Prequest[] requests)
        throws MPIException;

/**
 * Set the request object to be void.
 * Java binding of the MPI operation {@code MPI_REQUEST_FREE}.
 */
@Override public void free() throws MPIException
{
    buf = null;
    super.free();
}

} // Prequest
